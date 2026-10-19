#include "Test/Public/HealthComponentSpec.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"

BEGIN_DEFINE_SPEC(FHealthComponentSpec, "Proyecto.Components.HealthComponent", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext)
AActor* TestActor;
UHealthComponent* HealthComp;
END_DEFINE_SPEC(FHealthComponentSpec)

void FHealthComponentSpec::Define()
{
    BeforeEach([this]()
        {
            // Obtenemos el mundo actual de ejecución del editor/juego
            UWorld* CurrentWorld = GWorld;

            if (CurrentWorld)
            {
                // Spawn oficial de un Actor dentro del mundo de Unreal
                TestActor = CurrentWorld->SpawnActor<AActor>();
            }

            // Creamos el componente y lo asociamos al Actor
            HealthComp = NewObject<UHealthComponent>(TestActor, UHealthComponent::StaticClass(), TEXT("HealthComponent"));

            if (HealthComp)
            {
                HealthComp->RegisterComponent();
            }
        });

    AfterEach([this]()
        {
            // Limpieza de memoria después de cada test
            if (TestActor)
            {
                TestActor->Destroy();
                TestActor = nullptr;
                HealthComp = nullptr;
            }
        });

    Describe("Initialization", [this]()
        {
            It("Should initialize with maximum health by default", [this]()
                {
                    TestNotNull("HealthComponent should exist", HealthComp);
                    TestEqual("Current health should equal max health", HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
                });
        });

    Describe("Taking Damage", [this]()
        {
            It("Should reduce current health correctly when taking damage", [this]()
                {
                    const float InitialHealth = HealthComp->GetCurrentHealth();
                    const float DamageAmount = 25.0f;

                    HealthComp->ApplyDamage(DamageAmount);

                    TestEqual("Health should be reduced by damage amount", HealthComp->GetCurrentHealth(), InitialHealth - DamageAmount);
                });

            It("Should not allow health to drop below zero", [this]()
                {
                    const float MassiveDamage = 9999.0f;

                    HealthComp->ApplyDamage(MassiveDamage);

                    TestEqual("Health should not drop below 0", HealthComp->GetCurrentHealth(), 0.0f);
                });

            It("Should trigger death event or state when health reaches zero", [this]()
                {
                    HealthComp->ApplyDamage(HealthComp->GetMaxHealth());

                    TestTrue("IsDead should be true when health is zero", HealthComp->IsDead());
                });
        });

    Describe("Healing", [this]()
        {
            It("Should increase health when healed", [this]()
                {
                    HealthComp->ApplyDamage(50.0f);
                    const float DamagedHealth = HealthComp->GetCurrentHealth();
                    const float HealAmount = 20.0f;

                    HealthComp->ApplyHealing(HealAmount);

                    TestEqual("Health should increase by heal amount", HealthComp->GetCurrentHealth(), DamagedHealth + HealAmount);
                });

            It("Should not exceed maximum health when healed", [this]()
                {
                    const float MaxHealth = HealthComp->GetMaxHealth();
                    HealthComp->ApplyDamage(20.0f);

                    HealthComp->ApplyHealing(999.0f); // Curación excesiva

                    TestEqual("Health should be capped at MaxHealth", HealthComp->GetCurrentHealth(), MaxHealth);
                });
        });
    Describe("Blueprint Compatibility and Properties", [this]()
        {
            It("Should be Blueprintable and BlueprintType", [this]()
                {
                    UClass* Class = UHealthComponent::StaticClass();
                    TestNotNull("Class must exist", Class);

                    bool bIsBlueprintable = Class->HasAnyClassFlags(EClassFlags::CLASS_CompiledFromBlueprint) || Class->GetBoolMetaData(TEXT("IsBlueprintBase"));

                    TestTrue(TEXT("Class should be Blueprintable"), bIsBlueprintable);

                    // Verifica si la clase está permitida como tipo de variable en Blueprints
                    bool bIsBlueprintType = UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Class);

                    TestTrue(TEXT("Class should be BlueprintType"), bIsBlueprintType);


                });

            It("Should have editable and blueprint-accessible variables", [this]()
                {
                    UClass* Class = UHealthComponent::StaticClass();

                    // 1. Validar la propiedad CurrentHealth (Visible por Instancia, NO Editable - Ej: VisibleAnywhere)
                    FProperty* DamageProp = Class->FindPropertyByName(TEXT("CurrentHealth"));
                    if (TestNotNull(TEXT("CurrentHealth property must exist via reflection"), DamageProp))
                    {
                        // Debe ser accesible en Blueprints (BlueprintReadOnly o BlueprintReadWrite)
                        TestTrue(TEXT("CurrentHealth must be accessible in Blueprints"),
                            DamageProp->HasAnyPropertyFlags(CPF_BlueprintVisible));

                        // El motor necesita CPF_Edit para dibujar la variable en los paneles del editor
                        bool bIsExposedInEditor = DamageProp->HasAnyPropertyFlags(CPF_Edit);

                        // Al usar 'VisibleAnywhere', el motor activa obligatoriamente 'CPF_EditConst' (Solo Lectura)
                        bool bIsReadOnlyInEditor = DamageProp->HasAnyPropertyFlags(CPF_EditConst);

                        // Nos aseguramos de que NO esté oculta en las instancias del nivel (Evitamos VisibleDefaultsOnly)
                        bool bIsDisabledOnInstance = DamageProp->HasAnyPropertyFlags(CPF_DisableEditOnInstance);

                        // Condición final para Solo Lectura expuesto en la instancia
                        bool bIsVisibleButReadOnlyOnInstance = bIsExposedInEditor && bIsReadOnlyInEditor && !bIsDisabledOnInstance;

                        TestTrue(TEXT("CurrentHealth must be visible but NOT editable by the Actor in the level"),
                            bIsVisibleButReadOnlyOnInstance);
                    }

                    // 2. Validar la propiedad MaxHealth (Totalmente Editable por Instancia - Ej: EditAnywhere)
                    FProperty* TimeProp = Class->FindPropertyByName(TEXT("MaxHealth"));
                    if (TestNotNull(TEXT("MaxHealth property must exist via reflection"), TimeProp))
                    {
                        // Debe ser editable en el editor de forma general
                        TestTrue(TEXT("MaxHealth must be editable in editor (CPF_Edit)"),
                            TimeProp->HasAnyPropertyFlags(CPF_Edit));

                        // Debe ser accesible en Blueprints
                        TestTrue(TEXT("MaxHealth must be accessible in Blueprints (CPF_BlueprintVisible)"),
                            TimeProp->HasAnyPropertyFlags(CPF_BlueprintVisible));

                        // Para ser editable POR INSTANCIA en el nivel, NO debe tener el flag de bloqueo en instancias
                        bool bIsDisabledOnInstance = TimeProp->HasAnyPropertyFlags(CPF_DisableEditOnInstance);
                        TestFalse(TEXT("MaxHealth must NOT disable edit on instance (should be editable in level)"),
                            bIsDisabledOnInstance);

                        // Nos aseguramos que NO sea de solo lectura
                        bool bIsReadOnly = TimeProp->HasAnyPropertyFlags(CPF_EditConst);
                        TestFalse(TEXT("MaxHealth must NOT be read-only"), bIsReadOnly);
                    }
                });
        });
}