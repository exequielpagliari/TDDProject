

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DamageZone.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TDDPROYECT_API ADamageZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageZone();
	ADamageZone(int Damage);
	ADamageZone(int Damage, int TimeDamage);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int DamageValue;
	int GetDamageValue() const { return DamageValue; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int TimeDamageValue;
	int GetTimeDamageValue() const { return TimeDamageValue; }
	void SetDamageValue(int Damage) { DamageValue = FMath::Clamp(Damage, 0, Damage);}
	void SetTimeDamageValue(int TimeDamage) { TimeDamageValue = FMath::Clamp(TimeDamage, 0, TimeDamage);}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnOverlapStay(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;
	
	
};
