// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "BaseEntity.h"
#include "EntityStats.h"
#include "EventDispatcher.h"
#include "ObjectPoolComponent.h"
#include "Projectile.h"
#include "ProjectileStats.h"
#include "Tile.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

UWeapon::UWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UProjectileStats* UWeapon::GetProjectileStats()
{
	if (IsEvolved)
		return EvolvedProjectileStats;
	
	return ProjectileStatsPerLevel[Level];
}

bool UWeapon::CanLevelUp()
{
	return !IsEvolved && Level < ProjectileStatsPerLevel.Num() - 1;
}


void UWeapon::IncreaseLevel(int32 Amount)
{
	int32 maxLevel =  ProjectileStatsPerLevel.Num() - 1;
	if (Level < maxLevel)
		Level = FMath::Min(maxLevel, Level + Amount);

	UE_LOG(LogTemp, Warning, TEXT("Weapon level increased to %i"), Level);
}

void UWeapon::Evolve()
{
	MilestoneUnlockedOnEvolve->OnMilestoneUnlocked();
	IsEvolved = true;
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
	TimeSinceLastBulletSpawned += DeltaTime;

	switch (GetProjectileStats()->BulletPattern)
	{
	case FBulletPattern::Default:
		ProcessDefaultWeaponFire();
		break;
	case FBulletPattern::SinWave:
		ProcessSinWeaponFire();
		break;
	case FBulletPattern::Shotgun:
		ProcessShotgunWeaponFire();
		break;
	case FBulletPattern::Gatling:
		ProcessGatlingWeaponFire();
		break;
	case FBulletPattern::Shockwave:
		ProcessShockwaveWeaponFire();
	}
}

void UWeapon::ProcessDefaultWeaponFire()
{
	if (TimeSinceLastShot >= 1.0f / (GetProjectileStats()->GetFireRate() * Entity->EntityStats->GetFireRateMultiplier()))
	{
		TimeSinceLastShot = 0.0f;
		FireProjectile();
	}
}

void UWeapon::ProcessSinWeaponFire()
{
	float ROF = 1.0f / (GetProjectileStats()->GetFireRate() * Entity->EntityStats->GetFireRateMultiplier());
	if (TimeSinceLastShot >= ROF)
	{
		if (TimeSinceLastBulletSpawned >= ROF / (GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier()))
			FireProjectile();
	}
}

void UWeapon::ProcessShotgunWeaponFire()
{
	ProcessDefaultWeaponFire();
}

void UWeapon::ProcessGatlingWeaponFire()
{
	float ROF = 1.0f / (GetProjectileStats()->GetFireRate() * Entity->EntityStats->GetFireRateMultiplier());
	if (TimeSinceLastShot >= ROF)
	{
		if (TimeSinceLastBulletSpawned >= 1 / (GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier()))
			FireProjectile();
	}
}

void UWeapon::ProcessShockwaveWeaponFire()
{
	float ROF = 1.0f / (GetProjectileStats()->GetFireRate() * Entity->EntityStats->GetFireRateMultiplier() * Entity->EntityStats->GetProjectileCountMultiplier());

	if (TimeSinceLastShot >= ROF)
	{
		float ratio = (TimeSinceLastShot - ROF) * 2.0f;
		float shockwaveSize = GetProjectileStats()->GetExplosionSize() * Entity->EntityStats->GetExplosionSizeMultiplier();
		
		if (ratio > 1.0f)
		{
			ratio = 1.0f;
			TimeSinceLastShot = 0.0f;
			SpawnTilesAroundActor(shockwaveSize * ratio / 2.0f);
			DamageEnemiesAroundActor(shockwaveSize * ratio / 2.0f);
			EnemyHitThisWave.Empty();
			TileHitThisWave.Empty();
			return;
		}
		SpawnTilesAroundActor(shockwaveSize * ratio / 2.0f);
		DamageEnemiesAroundActor(shockwaveSize * ratio / 2.0f);
	}
}

void UWeapon::FireProjectile()
{
	switch (GetProjectileStats()->BulletPattern)
	{
	case FBulletPattern::Default:
		FireDefaultProjectile();
		break;
	case FBulletPattern::SinWave:
		FireSinProjectile();
		break;
	case FBulletPattern::Shotgun:
		FireShotgunProjectile();
		break;
	case FBulletPattern::Gatling:
		FireGatlingProjectile();
		break;
	case FBulletPattern::Shockwave:
		FireShockwaveProjectile();
		break;
	}
}

void UWeapon::FireDefaultProjectile()
{
	FVector actorLocation = Entity->GetActorLocation();
	FRotator actorRotation = Entity->ProjectileSpawnPoint->GetComponentRotation();

	int32 projectileCount = GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier();
	float angle = 360 / projectileCount;
	for (int32 i = 0; i < projectileCount; i++)
	{
		FVector spawnOffset = Entity->ProjectileSpawnPoint->GetComponentLocation() - actorLocation;
		FRotator rotationOffset = FRotator(0, angle * i, 0);
		spawnOffset = rotationOffset.RotateVector(spawnOffset);
		
		SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset, actorRotation + rotationOffset);
	}
}

void UWeapon::FireSinProjectile()
{
	FVector actorLocation = Entity->GetActorLocation();
	FRotator actorRotation = Entity->ProjectileSpawnPoint->GetComponentRotation();

	FVector spawnOffset = Entity->ProjectileSpawnPoint->GetComponentLocation() - actorLocation;
	int32 ShotCount = GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier();
	int32 variant = ShotCounter % ShotCount;
	if (ShotCounter > ShotCount)
	{
		TimeSinceLastShot = 0.0f;
		ShotCounter = 0;
	}
	ShotCounter++;
	variant--;

	float piSlice = 2 * PI / (GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier());

	float sinValue = FMath::Sin(piSlice * variant);
	
	FRotator rotationOffset = FRotator(0, GetProjectileStats()->BulletPatternArcWidth * sinValue, 0);
	spawnOffset = rotationOffset.RotateVector(spawnOffset);

	SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset, actorRotation + rotationOffset);
	TimeSinceLastBulletSpawned = 0.0f;
}

void UWeapon::FireShotgunProjectile()
{
	FVector actorLocation = Entity->GetActorLocation();
	FRotator actorRotation = Entity->ProjectileSpawnPoint->GetComponentRotation();

	FVector spawnOffset = Entity->ProjectileSpawnPoint->GetComponentLocation() - actorLocation;

	int32 ShotCount = GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier();

	float angle = GetProjectileStats()->BulletPatternArcWidth / (ShotCount / 2);
	
	if (ShotCount % 2 != 0)
	{
		SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset, actorRotation);
		ShotCount--;
		
		for (int32 i = 1; i <= ShotCount / 2; i++)
		{
			FVector spawnOffset2 = spawnOffset;
			FRotator rotationOffset = FRotator(0, angle * i, 0);
			spawnOffset2 = rotationOffset.RotateVector(spawnOffset2);
			SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset2, actorRotation + rotationOffset);
		}

		for (int32 i = -1; i >= -ShotCount / 2; i--)
		{
			FVector spawnOffset2 = spawnOffset;
			FRotator rotationOffset = FRotator(0, angle * i, 0);
			spawnOffset2 = rotationOffset.RotateVector(spawnOffset2);
			SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset2, actorRotation + rotationOffset);
		}
	}
	else
	{
		for (int32 i = 1; i <= ShotCount / 2; i++)
		{
			FVector spawnOffset2 = spawnOffset;
			FRotator rotationOffset = FRotator(0, angle * i - (angle / 2), 0);
			spawnOffset2 = rotationOffset.RotateVector(spawnOffset2);
			SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset2, actorRotation + rotationOffset);
		}

		for (int32 i = -1; i >= -ShotCount / 2; i--)
		{
			FVector spawnOffset2 = spawnOffset;
			FRotator rotationOffset = FRotator(0, angle * i + (angle / 2), 0);
			spawnOffset2 = rotationOffset.RotateVector(spawnOffset2);
			SpawnBulletAtPositionWithRotation(actorLocation + spawnOffset2, actorRotation + rotationOffset);
		}
	}
}

void UWeapon::FireGatlingProjectile()
{
	FVector spawnLocation = Entity->ProjectileSpawnPoint->GetComponentLocation();
	FRotator spawnRotation = Entity->ProjectileSpawnPoint->GetComponentRotation();

	int32 ShotCount = GetProjectileStats()->GetProjectileCount() * Entity->EntityStats->GetProjectileCountMultiplier();

	if (ShotCounter > ShotCount)
	{
		TimeSinceLastShot = 0.0f;
		ShotCounter = 0;
	}
	ShotCounter++;
	
	SpawnBulletAtPositionWithRotation(spawnLocation, spawnRotation);
	TimeSinceLastBulletSpawned = 0.0f;
}

void UWeapon::FireShockwaveProjectile()
{
	
}

void UWeapon::SpawnBulletAtPositionWithRotation(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	AProjectile* Projectile = Entity->GameMode->GetObjectPool()->GetFromPool<AProjectile>(GetProjectileStats()->ProjectileClass, SpawnLocation, SpawnRotation);
		
	if (!Projectile)
		return;

	Projectile->SetOwner(Entity);
	Projectile->SetActorScale3D(FVector(GetProjectileStats()->GetProjectileScale() * Entity->EntityStats->GetProjectileSizeMultiplier()));
	Projectile->OnGetFromPool(GetProjectileStats(), Entity->EntityStats, ShotAlternator);
}

void UWeapon::SpawnTilesAroundActor(float Radius)
{
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(Radius);
	FVector actorPos = Entity->GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(TileHitThisWave);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			ATile* tile = Cast<ATile>(actor);
			if (actor && tile)
			{
				TileHitThisWave.Add(actor);
				tile->SetElement(GetProjectileStats()->Element);
			}
		}
	}
}

void UWeapon::DamageEnemiesAroundActor(float Radius)
{
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(Radius);
	FVector actorPos = Entity->GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Entity);
	QueryParams.AddIgnoredActors(EnemyHitThisWave);
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			if (actor && actor != Entity)
			{
				EnemyHitThisWave.Add(actor);
				UGameplayStatics::ApplyDamage(actor, GetProjectileStats()->GetDamageAmount() * Entity->EntityStats->GetDamageMultiplier(), GetOwner()->GetInstigatorController(), Entity, GetProjectileStats()->DamageType);
			}
		}
	}
}
