// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TDDProjectGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class TDDPROYECT_API UTDDProjectGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:

    UPROPERTY(Config)
    float MusicVolume = 1.0f;

    UPROPERTY(Config)
    float SFXVolume = 1.0f;
};
