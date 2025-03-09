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
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetDamageAmount();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileSpeed();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileLifetime();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	int32 GetProjectileCount();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileScale();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetExplosionDamageAmount();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetExplosionSize();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetKnockbackAmount();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	int32 GetProjectilePenetrations();
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	bool GetInfinitePenetrations();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	EElementalType Element;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	TSubclassOf<UDamageType> DamageType;
	

protected:
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

};
