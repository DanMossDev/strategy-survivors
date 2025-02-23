// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "ProjectileStats.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UProjectileStats : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float DamageAmount = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float ProjectileSpeed = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float ProjectileLifetime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	int32 ProjectileCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float ProjectileScale = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float ExplosionDamageAmount = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float ExplosionSize = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float KnockbackAmount = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	int32 ProjectilePenetrations = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	bool InfinitePentrations = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	EElementalType Element;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	TSubclassOf<class APuddle> Puddle;
};
