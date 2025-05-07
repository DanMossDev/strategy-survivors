// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStats.h"
#include "StatusEffectComponent.h"

void UEntityStats::InjectStatusEffectComponent(UStatusEffectComponent* StatusEffectComp)
{
	StatusEffectComponent = StatusEffectComp;
}

void UEntityStats::OverrideWith(const UEntityStats* Other)
{
	MaxHealth = Other->MaxHealth;
	MovementSpeed = Other->MovementSpeed;
	RotationSpeed = Other->RotationSpeed;

	HitInvincibilityTime = Other->HitInvincibilityTime;
	ContactDamageAmount = Other->ContactDamageAmount;
	PickupRange = Other->PickupRange;
	PickupRangeMultiplier = Other->PickupRangeMultiplier;
	
	DamageMultiplier = Other->DamageMultiplier;
	FireRateMultiplier = Other->FireRateMultiplier;
	ProjectileSizeMultiplier = Other->ProjectileSizeMultiplier;
	ProjectileSpeedMultiplier = Other->ProjectileSpeedMultiplier;
	ProjectileLifetimeMultiplier = Other->ProjectileLifetimeMultiplier;
	KnockbackAmount = Other->KnockbackAmount;
	KnockbackMultiplier = Other->KnockbackMultiplier;
	ExplosionDamageMultiplier = Other->ExplosionDamageMultiplier;
	ExplosionSizeMultiplier = Other->ExplosionSizeMultiplier;
	AttackRangeMultiplier = Other->AttackRangeMultiplier;
	ProjectileCountMultiplier = Other->ProjectileCountMultiplier;
	ProjectilePenetrationMultiplier = Other->ProjectileCountMultiplier;
}

void UEntityStats::SetTemporaryStats(const UEntityStats* TemporaryStats)
{
	TempMovementSpeed = TemporaryStats->TempMovementSpeed;
	TempPickupRange = TemporaryStats->TempPickupRange;
	
	TempDamageMultiplier = TemporaryStats->DamageMultiplier;
	TempFireRateMultiplier = TemporaryStats->FireRateMultiplier;
	TempProjectileSizeMultiplier = TemporaryStats->ProjectileSizeMultiplier;
	TempProjectileSpeedMultiplier = TemporaryStats->ProjectileSpeedMultiplier;
	TempProjectileLifetimeMultiplier = TemporaryStats->ProjectileLifetimeMultiplier;
	TempKnockbackAmount = TemporaryStats->KnockbackAmount;
	TempExplosionDamageMultiplier = TemporaryStats->ExplosionDamageMultiplier;
	TempExplosionSizeMultiplier = TemporaryStats->ExplosionSizeMultiplier;
	TempAttackRangeMultiplier = TemporaryStats->AttackRangeMultiplier;
	TempProjectileCountMultiplier = TemporaryStats->ProjectileCountMultiplier;
	TempProjectilePenetrationMultiplier = TemporaryStats->ProjectilePenetrationMultiplier;

	TempBurnDamagePerTick = TemporaryStats->BurnDamagePerTick;
	TempHealthRegenSpeed = TemporaryStats->HealthRegenSpeed;
}

void UEntityStats::ResetTemporaryStats()
{
	TempMovementSpeed = 0;
	TempPickupRange = 0;
	
	TempDamageMultiplier = 0;
	TempFireRateMultiplier = 0;
	TempProjectileSizeMultiplier = 0;
	TempProjectileSpeedMultiplier = 0;
	TempProjectileLifetimeMultiplier = 0;
	TempKnockbackAmount = 0;
	TempExplosionDamageMultiplier = 0;
	TempExplosionSizeMultiplier = 0;
	TempAttackRangeMultiplier = 0;
	TempProjectileCountMultiplier = 0;
	TempProjectilePenetrationMultiplier = 0;

	TempBurnDamagePerTick = 0;
	TempHealthRegenSpeed = 0;
}

void UEntityStats::SetAllToZero()
{
	MaxHealth = 0;
	MovementSpeed = 0;
	RotationSpeed = 0;

	HitInvincibilityTime = 0;
	ContactDamageAmount = 0;
	PickupRange = 0;
	PickupRangeMultiplier = 0;
	
	DamageMultiplier = 0;
	FireRateMultiplier = 0;
	ProjectileSizeMultiplier = 0;
	ProjectileSpeedMultiplier = 0;
	ProjectileLifetimeMultiplier = 0;
	KnockbackAmount = 0;
	KnockbackMultiplier = 0;
	ExplosionDamageMultiplier = 0;
	ExplosionSizeMultiplier = 0;
	AttackRangeMultiplier = 0;
	ProjectileCountMultiplier = 0;
	ProjectilePenetrationMultiplier = 0;

	BurnDamagePerTick = 0;
	HealthRegenSpeed = 0;
}

float UEntityStats::GetMovementSpeed() const
{
	float multiplier = 1.0f;
	if (StatusEffectComponent)
	{
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Wet))
		{
			multiplier = 0.75f;
		}
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Slowed))
		{
			multiplier = 0.5f;
		}
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		{
			multiplier = 0.0f;
		}
	}
	return (MovementSpeed + TempMovementSpeed) * multiplier;
}

float UEntityStats::GetRotationSpeed() const
{
	if (StatusEffectComponent)
	{
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		{
			return 0.0f;
		}
	}
	return RotationSpeed;
}

float UEntityStats::GetContactDamageAmount() const
{
	if (StatusEffectComponent)
	{
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		{
			return 0.0f;
		}
	}
	return ContactDamageAmount;
}

float UEntityStats::GetKnockbackAmount() const
{
	if (StatusEffectComponent)
	{
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		{
			return 0.0f;
		}
	}
	return (KnockbackAmount + TempKnockbackAmount) * KnockbackMultiplier;
}

float UEntityStats::GetDamageMultiplier() const
{
	return DamageMultiplier + TempDamageMultiplier;
}

float UEntityStats::GetFireRateMultiplier() const
{
	return FireRateMultiplier + TempFireRateMultiplier;
}

void UEntityStats::AddStats(const UEntityStats& incomingStats)
{
	MaxHealth += incomingStats.GetMaxHealth();
	MovementSpeed += incomingStats.GetMovementSpeed();
	RotationSpeed += incomingStats.GetRotationSpeed();
	HitInvincibilityTime += incomingStats.GetHitInvincibilityTime();
	ContactDamageAmount += incomingStats.GetContactDamageAmount();
	PickupRange += incomingStats.GetPickupRange();
	DamageMultiplier += incomingStats.GetDamageMultiplier();
	FireRateMultiplier += incomingStats.GetFireRateMultiplier();
	ProjectileSizeMultiplier += incomingStats.GetProjectileSizeMultiplier();
	ProjectileSpeedMultiplier += incomingStats.GetProjectileSpeedMultiplier();
	ProjectileLifetimeMultiplier += incomingStats.GetProjectileLifetimeMultiplier();
	KnockbackAmount += incomingStats.GetKnockbackAmount();
	KnockbackMultiplier += incomingStats.GetKnockbackMultiplier();
	ExplosionDamageMultiplier += incomingStats.GetExplosionDamageMultiplier();
	ExplosionSizeMultiplier += incomingStats.GetExplosionSizeMultiplier();
	AttackRangeMultiplier += incomingStats.GetAttackRangeMultiplier();
	ProjectileCountMultiplier += incomingStats.GetProjectileCountMultiplier();
	ProjectilePenetrationMultiplier += incomingStats.GetProjectilePenetrationMultiplier();
}
