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
	UEntityStats& operator+=(const UEntityStats& other)
	{
		this->AddStats(other);
		return *this;
	}
	
	void InjectStatusEffectComponent(class UStatusEffectComponent* StatusEffectComp);
	void OverrideWith(UEntityStats* Other);
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMovementSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetRotationSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetHitInvincibilityTime() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetContactDamageAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetPickupRange() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetDamageMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetFireRateMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSizeMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSpeedMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileLifetimeMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionDamageMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionSizeMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetAttackRangeMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	int32 GetProjectileCountMultiplier() const;

	void AddStats(const UEntityStats& incomingStats);

	int32 GetBurnDamagePerTick() const {return BurnDamagePerTick;}
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float PickupRange = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float PickupRangeMultiplier = 1.0f;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	int32 BurnDamagePerTick = 5;


	UStatusEffectComponent* StatusEffectComponent;
};
