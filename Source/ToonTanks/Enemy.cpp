// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "HealthComponent.h"
#include "Pickup.h"
#include "ObjectPoolComponent.h"
#include "PoolableComponent.h"
#include "StatusEffectComponent.h"
#include "ToonTanksGameMode.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
}

void AEnemy::OnGetFromPool()
{
	StatusEffectComponent->Init(this);
	HealthComponent->Init(EntityStats);
	FVector footAdjustedPosition = GetActorLocation();
	footAdjustedPosition.Z = CapsuleComponent->GetScaledCapsuleHalfHeight() + 1;
	SetActorLocation(footAdjustedPosition);
}

void AEnemy::OnReturnToPool()
{
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);

	for (auto weapon : Weapons)
	{
		weapon->SetComponentTickEnabled(false);
	}

	PoolableComponent->OnGetFromPool.AddDynamic(this, &AEnemy::OnGetFromPool);
	PoolableComponent->OnReturnToPool.AddDynamic(this, &AEnemy::OnReturnToPool);
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor) return;
}

void AEnemy::OnDeath()
{
	Super::OnDeath();

	SpawnRandomPickup();
	PoolableComponent->ReturnToPool();
}

void AEnemy::SpawnRandomPickup()
{
	float roll = FMath::RandRange(0.0f, 1.0f);
	for (auto kvp : WeightedPickupPool)
	{
		if (roll <= kvp.Key)
		{
			GameMode->GetObjectPool()->GetFromPool(kvp.Value, GetActorLocation(), GetActorRotation());
		}
	}
}
