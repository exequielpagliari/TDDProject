// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSubsystem.h"
#include "TDDProjectGameUserSettings.h"

UAudioSubsystem::UAudioSubsystem()
{
}

void UAudioSubsystem::SetMusicVolume(float Volume)
{
	MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

float UAudioSubsystem::GetMusicVolume() const
{
	return MusicVolume;
}

void UAudioSubsystem::SetSFXVolume(float Volume)
{
	SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

float UAudioSubsystem::GetSFXVolume() const
{
	return SFXVolume;
}

void UAudioSubsystem::ApplySettings(UTDDProjectGameUserSettings* UserSettings)
{
	if (UserSettings)
	{
		SetMusicVolume(UserSettings->MusicVolume);
		SetSFXVolume(UserSettings->SFXVolume);
	}
}
