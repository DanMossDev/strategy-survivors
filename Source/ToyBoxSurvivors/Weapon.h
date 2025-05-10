// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.generated.h"

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UWeapon : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeapon();

	class UProjectileStats* GetProjectileStats();
	bool CanLevelUp();
	void IncreaseLevel(int32 Amount);

	void Evolve();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ProcessFireWeapon(float DeltaTime);
	void FireProjectile();
	
	float TimeSinceLastShot = 0.0f;
	float TimeSinceLastBulletSpawned = 0.0f;

	bool ShotAlternator = false;
	int32 ShotCounter = 0;

	UPROPERTY()
	class ABaseEntity* Entity;
	UPROPERTY()
	class ATank* Player;

	int32 Level = 0;
	bool IsEvolved = false;

	bool HasFiredThisShockwave = false;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
	TArray<UProjectileStats*> ProjectileStatsPerLevel;

	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
	UProjectileStats* EvolvedProjectileStats;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UMilestone* MilestoneUnlockedOnEvolve;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool CanBulletsBounce = false;

	UPROPERTY()
	TArray<AActor*> EnemyHitThisWave = TArray<AActor*>();
	UPROPERTY()
	TArray<AActor*> TileHitThisWave = TArray<AActor*>();
	
	//Default
	void ProcessDefaultWeaponFire(const float DeltaTime);
	void FireDefaultProjectile();
	//Sin
	void ProcessSinWeaponFire(const float DeltaTime);
	void FireSinProjectile();
	//Shotgun
	void ProcessShotgunWeaponFire(const float DeltaTime);
	void FireShotgunProjectile();
	//Gatling
	void ProcessGatlingWeaponFire(const float DeltaTime);
	void FireGatlingProjectile();
	//Shockwave
	void ProcessShockwaveWeaponFire(const float DeltaTime);
	void FireShockwaveProjectile();
	//Crossfire
	void ProcessCrossfireWeaponFire(const float DeltaTime);
	void FireCrossfireProjectile();

	void SpawnBulletAtPositionWithRotation(const FVector& SpawnLocation, const FRotator& SpawnRotation);

	void SpawnTilesAroundActor(float Radius);
	void DamageEnemiesAroundActor(float Radius);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
