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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ProcessFireWeapon(float DeltaTime);
	void FireProjectile();

	UPROPERTY(EditAnywhere, Category = "Weapon") //move onto proj stats - also add a "fire pattern" enum that defaults to current one but allows shooting from sides, forward in a pattern, etc, maybe including extra components to add at levels?
	float BaseFireRate = 1.0f;
	float TimeSinceLastShot = 0.0f;

	bool ShotAlternator = false;

	class ABaseEntity* Entity;

	int32 Level = 0;
	bool IsEvolved = false;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
	TArray<UProjectileStats*> ProjectileStatsPerLevel;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
