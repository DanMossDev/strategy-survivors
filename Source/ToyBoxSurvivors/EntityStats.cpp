// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStats.h"
#include "StatusEffectComponent.h"

void UEntityStats::InjectStatusEffectComponent(UStatusEffectComponent* StatusEffectComp)
{
	StatusEffectComponent = StatusEffectComp;
}

void UEntityStats::OverrideWith(UEntityStats* Other)
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
	return MovementSpeed * multiplier;
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
	return KnockbackAmount * KnockbackMultiplier;
}

float UEntityStats::GetDamageMultiplier() const
{
	return DamageMultiplier;
}

float UEntityStats::GetFireRateMultiplier() const
{
	if (StatusEffectComponent)
	{
		if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		{
			return 0.0f;
		}
	}
	return FireRateMultiplier;
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
