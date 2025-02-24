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
	void InjectStatusEffectComponent(class UStatusEffectComponent* StatusEffectComp);
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMovementSpeed();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetRotationSpeed();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetHitInvincibilityTime();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetContactDamageAmount();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackAmount();
	
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetDamageMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetFireRateMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSizeMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSpeedMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileLifetimeMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionDamageMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionSizeMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetAttackRangeMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	int32 GetProjectileCountMultiplier();
	
protected:
	//Character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float MovementSpeed = 250.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float RotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float HitInvincibilityTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ContactDamageAmount = 1.0f;
	
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
	float KnockbackAmount = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float KnockbackMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ExplosionDamageMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ExplosionSizeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float AttackRangeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	int32 ProjectileCountMultiplier = 1;


	UStatusEffectComponent* StatusEffectComponent;
};
