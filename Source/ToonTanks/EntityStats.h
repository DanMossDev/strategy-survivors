// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EntityStats.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UEntityStats : public UObject
{
	GENERATED_BODY()

public:
	//Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float MovementSpeed = 250.0f;

	//Projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float DamageMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float FireRateMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ProjectileSizeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ProjectileSpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ProjectileLifetimeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ExplosionSizeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float AttackRangeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	int32 ProjectileCountMultiplier = 1;
};
