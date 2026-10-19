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
}