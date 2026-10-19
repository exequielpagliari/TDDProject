


#include "DamageZone.h"
#include "HealthComponent.h"


// Sets default values
ADamageZone::ADamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageValue = FMath::Clamp(10, 0, 10); // Valor de daño por defecto
	TimeDamageValue = FMath::Clamp(5, 0, 5); // Valor de daño por tiempo por defecto
	// 1. Crear el componente de caja
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	// 2. Hacerlo el componente raíz del Actor (para que tenga posición en el mundo)
	RootComponent = BoxComponent;

	// 3. Configurar un tamaño inicial para la caja (ej: 50x50x50 unidades)
	BoxComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// 4. Configurar el perfil de colisión para que actúe como Trigger (Overlap)
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));

	// 5. Vincular los eventos de solapamiento a tus funciones de C++
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);


}

ADamageZone::ADamageZone(int Damage)
{ 
	DamageValue = FMath::Clamp(Damage, 0, Damage); // Valor de daño personalizado
	TimeDamageValue = 5; // Valor de daño por tiempo por defecto
}

ADamageZone::ADamageZone(int Damage, int TimeDamage)
{
	DamageValue = FMath::Clamp(Damage, 0, Damage); // Valor de daño personalizado
	TimeDamageValue = FMath::Clamp(TimeDamage, 0, TimeDamage); // Valor de daño por tiempo personalizado
}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsValid(OtherActor);
	UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();
	HealthComp->ApplyDamage(DamageValue);
}

void ADamageZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ADamageZone::OnOverlapStay(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


