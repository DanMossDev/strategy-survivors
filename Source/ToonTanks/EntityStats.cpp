// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStats.h"
#include "StatusEffectComponent.h"

void UEntityStats::InjectStatusEffectComponent(UStatusEffectComponent* StatusEffectComp)
{
	StatusEffectComponent = StatusEffectComp;
}


float UEntityStats::GetMaxHealth() const
{
	return MaxHealth;
}

float UEntityStats::GetMovementSpeed() const
{
	float multiplier = 1.0f;
	if (StatusEffectComponent)
	{
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

float UEntityStats::GetHitInvincibilityTime() const
{
	return HitInvincibilityTime;
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

float UEntityStats::GetPickupRange() const
{
	return PickupRange * PickupRangeMultiplier;
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

float UEntityStats::GetProjectileSizeMultiplier() const
{
	return ProjectileSizeMultiplier;
}

float UEntityStats::GetProjectileSpeedMultiplier() const
{
	return ProjectileSpeedMultiplier;
}

float UEntityStats::GetProjectileLifetimeMultiplier() const
{
	return ProjectileLifetimeMultiplier;
}

float UEntityStats::GetKnockbackMultiplier() const
{
	return KnockbackMultiplier;
}

float UEntityStats::GetExplosionDamageMultiplier() const
{
	return ExplosionDamageMultiplier;
}

float UEntityStats::GetExplosionSizeMultiplier() const
{
	return ExplosionSizeMultiplier;
}

float UEntityStats::GetAttackRangeMultiplier() const
{
	return AttackRangeMultiplier;
}

int32 UEntityStats::GetProjectileCountMultiplier() const
{
	return ProjectileCountMultiplier;
}
