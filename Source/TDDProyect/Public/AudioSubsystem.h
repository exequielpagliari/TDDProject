// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TDDPROYECT_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UAudioSubsystem();
	void SetMusicVolume(float Volume);
	float GetMusicVolume() const;
	void SetSFXVolume(float Volume);
	float GetSFXVolume() const;
private:
	float MusicVolume;
	float SFXVolume;
};
