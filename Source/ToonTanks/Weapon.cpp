// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "BaseEntity.h"
#include "EntityStats.h"
#include "ObjectPoolComponent.h"
#include "Projectile.h"
#include "ProjectileStats.h"
#include "ToonTanksGameMode.h"

UWeapon::UWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeapon::BeginPlay()
{
	Super::BeginPlay();

	Entity = Cast<ABaseEntity>(GetOwner());
}

void UWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessFireWeapon(DeltaTime);
}

void UWeapon::ProcessFireWeapon(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;

	if (TimeSinceLastShot >= 1.0f / (BaseFireRate * Entity->EntityStats->FireRateMultiplier))
	{
		TimeSinceLastShot = 0.0f;
		FireProjectile();
	}
}

void UWeapon::FireProjectile()
{
	FVector actorLocation = Entity->GetActorLocation();
	FRotator actorRotation = Entity->ProjectileSpawnPoint->GetComponentRotation();

	int32 projectileCount = ProjectileStats->ProjectileCount * Entity->EntityStats->ProjectileCountMultiplier;
	float angle = 360 / projectileCount;
	for (int32 i = 0; i < projectileCount; i++)
	{
		FVector spawnOffset = Entity->ProjectileSpawnPoint->GetComponentLocation() - actorLocation;
		FRotator rotationOffset = FRotator(0, angle * i, 0);
		spawnOffset = rotationOffset.RotateVector(spawnOffset);
		
		AProjectile* Projectile = Entity->GameMode->GetObjectPool()->GetFromPool<AProjectile>(ProjectileClass, actorLocation + spawnOffset, actorRotation + rotationOffset);
		
		if (!Projectile)
			return;

		Projectile->SetOwner(Entity);
		Projectile->SetActorScale3D(FVector(ProjectileStats->ProjectileScale * Entity->EntityStats->ProjectileSizeMultiplier));
		Projectile->OnGetFromPool(ProjectileStats, Entity->EntityStats);
	}
}
