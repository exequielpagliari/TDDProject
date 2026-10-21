


#include "Test/Public/DamageZoneSpec.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "DamageZone.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "CoreUObject.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "EdGraphSchema_K2.h"

// Definimos el spec del actor test
BEGIN_DEFINE_SPEC(FDamageZoneSpec, "TDDProject.Actors.DamageZone", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext)
UWorld* TestWorld;
ADamageZone* DamageZone;
END_DEFINE_SPEC(FDamageZoneSpec)

void FDamageZoneSpec::Define()
{
    BeforeEach([this]()
        {
            TestWorld = GWorld;
            if (TestWorld)
            {
                // Spawneamos la zona de daño en el mundo virtual de prueba
                FActorSpawnParameters SpawnParams;
                DamageZone = TestWorld->SpawnActor<ADamageZone>(ADamageZone::StaticClass(), SpawnParams);
            }
        });

    AfterEach([this]()
        {
            if (DamageZone)
            {
                DamageZone->Destroy();
                DamageZone = nullptr;
            }


        });

    Describe("Initialization", [this]()
        {
            It("Should successfully spawn with valid components", [this]()
                {
                    TestNotNull("DamageZone instance should exist", DamageZone);
                    TestNotNull("BoxComponent should be initialized", DamageZone->BoxComponent);
                });

            It("Should have a valid initial damage value", [this]()
                {
                    // Validamos que el daño inicial esté configurado correctamente según tu constructor
                    TestTrue("Damage value should be greater than zero", DamageZone->GetDamageValue() > 0);
                });

            It("Should have a valid initial time value", [this]()
                {
                    // Validamos que el daño inicial esté configurado correctamente según tu constructor
                    TestTrue("Time damage value should be greater than zero", DamageZone->GetTimeDamageValue() > 0);
                });
        });


    Describe("Validation and Bounds", [this]()
        {
            It("Should not allow negative damage values", [this]()
                {
                    // Intentamos asignar un valor negativo mediante un setter seguro
                    DamageZone->SetDamageValue(-50);

                    // Verificamos que el sistema lo haya rechazado o limitado a 0 (o un mínimo válido)
                    TestTrue("Damage value cannot be negative", DamageZone->GetDamageValue() >= 0);
                });

            It("Should not allow negative time damage values", [this]()
                {
                    // Intentamos asignar un tiempo de daño negativo
                    DamageZone->SetTimeDamageValue(-2);

                    // Verificamos que no sea negativo
                    TestTrue("Time damage value cannot be negative", DamageZone->GetTimeDamageValue() >= 0);
                });
        });

    Describe("Overlap Interactions", [this]()
        {
            It("Should handle overlap begin event safely", [this]()
                {
                    TestNotNull("DamageZone must exist for overlap test", DamageZone);

                    // Spawneamos un actor dummy para simular que entra en la zona de daño
                    FActorSpawnParameters SpawnParams;
                    AActor* DummyTarget = TestWorld->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);
                    UHealthComponent* InitialHealthComp = NewObject<UHealthComponent>(DummyTarget, UHealthComponent::StaticClass(), TEXT("HealthComponent"));
                    InitialHealthComp->RegisterComponent();
                    UHealthComponent* HealthComp = DummyTarget->FindComponentByClass<UHealthComponent>();
                    TestNotNull("Dummy target should spawn successfully", DummyTarget);
                    float InitialHealth = HealthComp->GetCurrentHealth();

                    // Simulamos el evento de solapamiento llamando a la función directamente
                    // (Pasamos nullptr o componentes simulados en los parámetros que no requieran físicas complejas)
                    DamageZone->OnOverlapBegin(
                        DamageZone->BoxComponent,
                        DummyTarget,
                        nullptr,
                        0,
                        false,
                        FHitResult()
                    );

                    // Aquí puedes agregar tus aserciones (por ejemplo, verificar si el target recibió daño 
                    // si tu DamageZone interactúa directamente con un HealthComponent del DummyTarget).
                    // 5. ¡Aserción final! Verificamos que la vida haya bajado gracias a la DamageZone
                    TestTrue("DummyTarget health should be reduced after overlap", HealthComp->GetCurrentHealth() < InitialHealth);

                    // Limpieza del dummy target
                    if (DummyTarget)
                    {
                        DummyTarget->Destroy();
                    }
                });
        });

    Describe("Blueprint Compatibility and Properties", [this]()
    {
        It("Should be Blueprintable and BlueprintType", [this]()
        {
            UClass* Class = ADamageZone::StaticClass();
            TestNotNull("Class must exist", Class);

            bool bIsBlueprintable = Class->HasAnyClassFlags(EClassFlags::CLASS_CompiledFromBlueprint) || Class->GetBoolMetaData(TEXT("IsBlueprintBase"));

            TestTrue(TEXT("Class should be Blueprintable"), bIsBlueprintable);

            // Verifica si la clase está permitida como tipo de variable en Blueprints
            bool bIsBlueprintType = UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Class);

            TestTrue(TEXT("Class should be BlueprintType"), bIsBlueprintType);


        });

        It("Should have editable and blueprint-accessible variables", [this]()
        {
            UClass* Class = ADamageZone::StaticClass();

            // 1. Validar la propiedad DamageValue
            FProperty* DamageProp = Class->FindPropertyByName(TEXT("DamageValue"));
            TestNotNull("DamageValue property must exist via reflection", DamageProp);
            if (DamageProp)
            {
                TestTrue("DamageValue must be editable in editor (EditAnywhere)", 
                    DamageProp->HasAnyPropertyFlags(CPF_Edit));
                TestTrue("DamageValue must be accessible in Blueprints (BlueprintReadWrite)", 
                    DamageProp->HasAnyPropertyFlags(CPF_BlueprintVisible));
            }

            // 2. Validar la propiedad TimeDamageValue
            FProperty* TimeProp = Class->FindPropertyByName(TEXT("TimeDamageValue"));
            TestNotNull("TimeDamageValue property must exist via reflection", TimeProp);
            if (TimeProp)
            {
                TestTrue("TimeDamageValue must be editable in editor", 
                    TimeProp->HasAnyPropertyFlags(CPF_Edit));
                TestTrue("TimeDamageValue must be accessible in Blueprints", 
                    TimeProp->HasAnyPropertyFlags(CPF_BlueprintVisible));
            }
        });
    });
}