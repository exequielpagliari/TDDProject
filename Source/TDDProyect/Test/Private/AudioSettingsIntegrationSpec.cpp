


#include "Test/Public/AudioSettingsIntegrationSpec.h"
#include "AudioSubsystem.h"
#include "TDDProjectGameUserSettings.h"
#include "Engine/GameInstance.h"



#if WITH_DEV_AUTOMATION_TESTS

BEGIN_DEFINE_SPEC(FAudioSettingsIntegrationSpec, "TDDProject.Integration.AudioUserSettings", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext)
UGameInstance* GameInstance;
UAudioSubsystem* AudioSubsystem;
UTDDProjectGameUserSettings* MockUserSettings; // Instancia temporal para el test
END_DEFINE_SPEC(FAudioSettingsIntegrationSpec)

void FAudioSettingsIntegrationSpec::Define()
{
    BeforeEach([this]()
        {
            GameInstance = NewObject<UGameInstance>();
            AudioSubsystem = NewObject<UAudioSubsystem>(GameInstance);


            MockUserSettings = NewObject<UTDDProjectGameUserSettings>();
        });

    AfterEach([this]()
        {
            AudioSubsystem = nullptr;
            GameInstance = nullptr;
            MockUserSettings = nullptr;
        });

    Describe("When initializing audio from user settings", [this]()
        {
            It("should correctly apply music and SFX volumes from settings to the subsystem", [this]()
                {

                    MockUserSettings->MusicVolume = 0.42f;
                    MockUserSettings->SFXVolume = 0.85f;


                    AudioSubsystem->ApplySettings(MockUserSettings);

                    TestEqual(
                        "AudioSubsystem Music Volume should match UserSettings",
                        AudioSubsystem->GetMusicVolume(),
                        0.42f
                    );

                    TestEqual(
                        "AudioSubsystem SFX Volume should match UserSettings",
                        AudioSubsystem->GetSFXVolume(),
                        0.85f
                    );
                });
        });
}

#endif // WITH_DEV_AUTOMATION_TESTS