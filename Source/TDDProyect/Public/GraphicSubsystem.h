// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GraphicSubsystem.generated.h"

UENUM(BlueprintType)
enum class EAllGraphicQualityCategory : uint8
{
    ResolutionScale,
    ViewDistance,
    AntiAliasing,
    Shadows,
    GlobalIllumination,
    Reflections,
    Textures,
    VisualEffects,
    PostProcessing,
    Foliage,
    Shading,
    Landscape
};

/**
 * 
 */
UCLASS()
class TDDPROYECT_API UGraphicSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
};
