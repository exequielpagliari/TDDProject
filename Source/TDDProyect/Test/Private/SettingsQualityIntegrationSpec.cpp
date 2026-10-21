

#include "Test/Public/SettingsQualityIntegrationSpec.h"
#include "GraphicSubsystem.h"
#include "Misc/AutomationTest.h"
#include "GameFramework/GameUserSettings.h"



#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FGameUserSettingsQualitySpec, "TDDProject.Integration.QualitySettings", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
UGameInstance* GameInstance;
UGraphicSubsystem* GraphicSubsystem;
UGameUserSettings* UserSettings;

END_DEFINE_SPEC(FGameUserSettingsQualitySpec)

void FGameUserSettingsQualitySpec::Define()
{
    BeforeEach([this]()
        {
            GameInstance = NewObject<UGameInstance>();
			GraphicSubsystem = NewObject<UGraphicSubsystem>(GameInstance);

            UserSettings = UGameUserSettings::GetGameUserSettings();
        });

    AfterEach([this]()
        {
            GameInstance = nullptr;
			GraphicSubsystem = nullptr;
            UserSettings = nullptr;
        });

    Describe("When adjusting graphic quality settings", [this]()
        {
            It("should correctly update overall scalability level and individual quality groups", [this]()
                {
                    if (!UserSettings)
                    {
                        TestTrue("GameUserSettings must be valid", false);
                        return;
                    }

                    // Arrange: Guardamos el nivel anterior para poder restaurarlo si fuera necesario
                    int32 OriginalQuality = UserSettings->GetOverallScalabilityLevel();

                    // Act: Establecemos la calidad general a "Epic" (Nivel 3)
                    UserSettings->SetOverallScalabilityLevel(3);

                    // Assert: Comprobamos que el nivel general cambió a 3
                    TestEqual("Overall scalability level should be set to 3 (Epic)", UserSettings->GetOverallScalabilityLevel(), 3);

                    // Verificamos que los subgrupos clave (como sombras o texturas) también se hayan propagado correctamente
                    FString ResolutionQualityStr;
                    int32 ShadowQuality = UserSettings->GetShadowQuality();
                    int32 TextureQuality = UserSettings->GetTextureQuality();

                    TestEqual("Shadow quality should correspond to Epic level", ShadowQuality, 3);
                    TestEqual("Texture quality should correspond to Epic level", TextureQuality, 3);

                    // Cleanup local del estado de calidad para no afectar otros tests
                    UserSettings->SetOverallScalabilityLevel(OriginalQuality);
                });

            It("should allow custom fine-tuning of individual scalability parameters", [this]()
                {
                    if (!UserSettings) return;

                    // Act: Modificamos un parámetro de calidad específico de forma independiente (ej. Efectos a Low = 0)
                    UserSettings->SetVisualEffectQuality(0);

                    // Assert: Comprobamos que el cambio individual se reflejó correctamente en el objeto
                    TestEqual("Visual effect quality should be Low (0)", UserSettings->GetVisualEffectQuality(), 0);
                });
        });
}

#endif // WITH_DEV_AUTOMATION_TESTS