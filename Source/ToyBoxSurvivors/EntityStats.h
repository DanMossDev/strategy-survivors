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
	void OverrideWith(const UEntityStats* Other);
	void SetTemporaryStats(const UEntityStats* TemporaryStats);
	void ResetTemporaryStats();
	void SetAllToZero();
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMaxHealth() const {return MaxHealth;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetMovementSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetRotationSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetHitInvincibilityTime() const {return HitInvincibilityTime;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetContactDamageAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetPickupRange() const {return PickupRange + TempPickupRange;}
	
	
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetDamageMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetFireRateMultiplier() const;
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSizeMultiplier() const {return ProjectileSizeMultiplier + TempProjectileSizeMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileSpeedMultiplier() const {return ProjectileSpeedMultiplier + TempProjectileSpeedMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileLifetimeMultiplier() const {return ProjectileLifetimeMultiplier + TempProjectileLifetimeMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetKnockbackMultiplier() const {return KnockbackMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionDamageMultiplier() const {return ExplosionDamageMultiplier + TempExplosionDamageMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetExplosionSizeMultiplier() const {return ExplosionSizeMultiplier + TempExplosionSizeMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetAttackRangeMultiplier() const {return AttackRangeMultiplier + TempAttackRangeMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectileCountMultiplier() const {return ProjectileCountMultiplier + TempProjectileCountMultiplier;}
	UFUNCTION(BlueprintCallable, Category = "Entity Stats")
	float GetProjectilePenetrationMultiplier() const {return ProjectilePenetrationMultiplier + TempProjectilePenetrationMultiplier;}

	void AddStats(const UEntityStats& incomingStats);

	int32 GetBurnDamagePerTick() const {return BurnDamagePerTick + TempBurnDamagePerTick;}

	float GetHealthRegen() const {return HealthRegenSpeed + TempHealthRegenSpeed;}
	
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
	float ProjectileCountMultiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float ProjectilePenetrationMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	int32 BurnDamagePerTick = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity Stats")
	float HealthRegenSpeed = 0.0f;

protected:
	//Temporary
	UPROPERTY()
	float TempMovementSpeed = 0.0f;
	UPROPERTY()
	float TempPickupRange = 0.0f;
	
	UPROPERTY()
	float TempDamageMultiplier = 0.0f;
	UPROPERTY()
	float TempFireRateMultiplier = 0.0f;
	UPROPERTY()
	float TempProjectileSizeMultiplier = 0.0f;
	UPROPERTY()
	float TempProjectileSpeedMultiplier = 0.0f;
	UPROPERTY()
	float TempProjectileLifetimeMultiplier = 0.0f;
	UPROPERTY()
	float TempKnockbackAmount = 0.0f;
	UPROPERTY()
	float TempExplosionDamageMultiplier = 0.0f;
	UPROPERTY()
	float TempExplosionSizeMultiplier = 0.0f;
	UPROPERTY()
	float TempAttackRangeMultiplier = 0.0f;
	UPROPERTY()
	float TempProjectileCountMultiplier = 0;
	UPROPERTY()
	float TempProjectilePenetrationMultiplier = 0;

	UPROPERTY()
	int32 TempBurnDamagePerTick = 0;

	UPROPERTY()
	float TempHealthRegenSpeed = 0.0f;

	UPROPERTY()
	UStatusEffectComponent* StatusEffectComponent;
};
