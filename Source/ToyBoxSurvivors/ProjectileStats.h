// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "ProjectileStats.generated.h"

UENUM(BlueprintType)
enum class FBulletPattern : uint8
{
	Default UMETA(DisplayName = "Default"),
	SinWave UMETA(DisplayName = "SinWave"),
	Shotgun UMETA(DisplayName = "Shotgun"),
};

UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UProjectileStats : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetFireRate() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetDamageAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileLifetime() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	int32 GetProjectileCount() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetProjectileScale() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetExplosionDamageAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetExplosionSize() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	float GetKnockbackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	int32 GetProjectilePenetrations() const;
	UFUNCTION(BlueprintCallable, Category = "Projectile Stats")
	bool GetInfinitePenetrations() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	EElementalType Element;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Projectile Stats")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile Stats")
	FBulletPattern BulletPattern;

	UPROPERTY(EditAnywhere, Category = "Projectile Stats")
	float BulletPatternArcWidth = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	bool IsHoming = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	bool IsOrbiting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	bool HasTrail = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	bool IsBoomerang = false;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Stats")
	float FireRate = 1.0f;
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
