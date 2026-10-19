// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDDPROYECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	void ApplyDamage(float DamageAmount);
	void ApplyHealing(float HealAmount);
	bool IsDead() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	float MaxHealth = 100.0f;
	float CurrentHealth = 100.0f;
	bool bIsDead = false;
		
};
