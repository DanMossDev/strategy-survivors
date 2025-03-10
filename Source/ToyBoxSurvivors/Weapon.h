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
	
	float TimeSinceLastShot = 0.0f;

	bool ShotAlternator = false;

	class ABaseEntity* Entity;

	int32 Level = 0;
	bool IsEvolved = false;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
	TArray<UProjectileStats*> ProjectileStatsPerLevel;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
