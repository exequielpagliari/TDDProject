#if WITH_DEV_AUTOMATION_TESTS

#include "Test/Public/AudioSubsytemSpec.h"
#include "Misc/AutomationTest.h"
#include "AudioSubsystem.h"
#include "Engine/GameInstance.h"

BEGIN_DEFINE_SPEC(
    FAudioSubsystemSpec,
    "TDDProject.AudioSubsystem", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext)

UGameInstance* GameInstance = nullptr;
UAudioSubsystem* AudioSubsystem = nullptr;

END_DEFINE_SPEC(FAudioSubsystemSpec)

void FAudioSubsystemSpec::Define()
{
    Describe(
        "Audio volume configuration",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    GameInstance =
                        NewObject<UGameInstance>(
                            GetTransientPackage());

                    AudioSubsystem =
                        NewObject<UAudioSubsystem>(
                            GameInstance);
                });

            Describe(
                "When changing the music volume",
                [this]()
                {
                    BeforeEach(
                        [this]()
                        {
                            AudioSubsystem->SetMusicVolume(1.0f);
                            AudioSubsystem->SetSFXVolume(1.0f);
                        });

                    It(
                        "should update music volume",
                        [this]()
                        {
                            AudioSubsystem->SetMusicVolume(0.35f);

                            TestTrue(
                                TEXT("Music volume should be 0.35"),
                                FMath::IsNearlyEqual(
                                    AudioSubsystem->GetMusicVolume(),
                                    0.35f));
                        });

                    It(
                        "should not change SFX volume",
                        [this]()
                        {
                            AudioSubsystem->SetMusicVolume(0.35f);

                            TestTrue(
                                TEXT("SFX volume should remain 1.0"),
                                FMath::IsNearlyEqual(
                                    AudioSubsystem->GetSFXVolume(),
                                    1.0f));
                        });
                });

            Describe(
                "When setting a volume outside its valid range",
                [this]()
                {
                    It(
                        "should clamp music volume to 1.0",
                        [this]()
                        {
                            AudioSubsystem->SetMusicVolume(1.5f);

                            TestTrue(
                                TEXT("Music volume should be clamped"),
                                FMath::IsNearlyEqual(
                                    AudioSubsystem->GetMusicVolume(),
                                    1.0f));
                        });

                    It(
                        "should clamp music volume to 0.0",
                        [this]()
                        {
                            AudioSubsystem->SetMusicVolume(-0.5f);

                            TestTrue(
                                TEXT("Music volume should be clamped"),
                                FMath::IsNearlyEqual(
                                    AudioSubsystem->GetMusicVolume(),
                                    0.0f));
                        });
                });
        

                 Describe(
                        "When changing the SFX volume",
                        [this]()
                        {
                            BeforeEach(
                                [this]()
                                {
                                    AudioSubsystem->SetMusicVolume(1.0f);
                                    AudioSubsystem->SetSFXVolume(1.0f);
                                });

                            It(
                                "should update SFX volume",
                                [this]()
                                {
                                    AudioSubsystem->SetSFXVolume(0.35f);

                                    TestTrue(
                                        TEXT("SFX volume should be 0.35"),
                                        FMath::IsNearlyEqual(
                                            AudioSubsystem->GetSFXVolume(),
                                            0.35f));
                                });

                            It(
                                "should not change music volume",
                                [this]()
                                {
                                    AudioSubsystem->SetSFXVolume(0.35f);

                                    TestTrue(
                                        TEXT("Music volume should remain 1.0"),
                                        FMath::IsNearlyEqual(
                                            AudioSubsystem->GetMusicVolume(),
                                            1.0f));
                                });
                        });

                    Describe(
                        "When setting an SFX volume outside its valid range",
                        [this]()
                        {
                            It(
                                "should clamp SFX volume to 1.0",
                                [this]()
                                {
                                    AudioSubsystem->SetSFXVolume(1.5f);

                                    TestTrue(
                                        TEXT("SFX volume should be clamped"),
                                        FMath::IsNearlyEqual(
                                            AudioSubsystem->GetSFXVolume(),
                                            1.0f));
                                });

                            It(
                                "should clamp SFX volume to 0.0",
                                [this]()
                                {
                                    AudioSubsystem->SetSFXVolume(-0.5f);

                                    TestTrue(
                                        TEXT("SFX volume should be clamped"),
                                        FMath::IsNearlyEqual(
                                            AudioSubsystem->GetSFXVolume(),
                                            0.0f));
                                });
                        });
                    });
}


#endif // WITH_DEV_AUTOMATION_TESTS