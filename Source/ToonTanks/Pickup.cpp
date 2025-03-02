// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "GravitateToPlayerInRange.h"
#include "PoolableComponent.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup()
{
	CollisionAndVisuals = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Graphics"));
	SetRootComponent(CollisionAndVisuals);
	
	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	PoolableComponent->OnGetFromPool.AddDynamic(this, &APickup::OnGetFromPool);
	PoolableComponent->OnReturnToPool.AddDynamic(this, &APickup::OnReturnToPool);
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));

	GravitateToPlayerInRangeComponent = GetComponentByClass<UGravitateToPlayerInRange>();
}

void APickup::OnGetFromPool()
{
	CollisionAndVisuals->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);

	FVector footAdjustedPosition = GetActorLocation();
	footAdjustedPosition.Z = CollisionAndVisuals->Bounds.SphereRadius;
	SetActorLocation(footAdjustedPosition);

	if (PickupType == EPickupType::XP)
		PickupAmount = GameMode->GetCurrentWave()->XPMultiplier * FMath::Pow(10.0f, PickupTier);
	if (PickupType == EPickupType::Health)
		PickupAmount = GameMode->GetCurrentWave()->HealthDropMultiplier * FMath::Pow(10.0f, PickupTier);

	if (GravitateToPlayerInRangeComponent)
		GravitateToPlayerInRangeComponent->Init();
}

void APickup::OnReturnToPool()
{
	if (CollisionAndVisuals->OnComponentBeginOverlap.IsAlreadyBound(this, &APickup::OnBeginOverlap))
		CollisionAndVisuals->OnComponentBeginOverlap.RemoveDynamic(this, &APickup::OnBeginOverlap);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GameMode)
	{
		PoolableComponent->ReturnToPool();
		UE_LOG(LogTemp, Warning, TEXT("PICKUP:: Didn't get game mode"));
		return;
	}
	
	switch (PickupType)
	{
	case EPickupType::XP:
		GameMode->PickupXP(PickupAmount);
		break;
	case EPickupType::Coin:
		GameMode->PickupCoin(PickupAmount);
		break;
	case EPickupType::Item:
		GameMode->PickupItem(PickupTier);
		break;
	case EPickupType::Health:
		GameMode->PickupHealth(PickupAmount);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Pickup type not recognized"));
		break;
	}
	OnPickedUp();
	PoolableComponent->ReturnToPool();
}
