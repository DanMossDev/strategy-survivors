// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityStats.h"
#include "StatusEffectComponent.h"

void UEntityStats::InjectStatusEffectComponent(UStatusEffectComponent* StatusEffectComp)
{
	StatusEffectComponent = StatusEffectComp;
}


float UEntityStats::GetMaxHealth()
{
	return MaxHealth;
}

float UEntityStats::GetMovementSpeed()
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

float UEntityStats::GetRotationSpeed()
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

float UEntityStats::GetHitInvincibilityTime()
{
	return HitInvincibilityTime;
}

float UEntityStats::GetContactDamageAmount()
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

float UEntityStats::GetKnockbackAmount()
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

float UEntityStats::GetDamageMultiplier()
{
	return DamageMultiplier;
}

float UEntityStats::GetFireRateMultiplier()
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

float UEntityStats::GetProjectileSizeMultiplier()
{
	return ProjectileSizeMultiplier;
}

float UEntityStats::GetProjectileSpeedMultiplier()
{
	return ProjectileSpeedMultiplier;
}

float UEntityStats::GetProjectileLifetimeMultiplier()
{
	return ProjectileLifetimeMultiplier;
}

float UEntityStats::GetKnockbackMultiplier()
{
	return KnockbackMultiplier;
}

float UEntityStats::GetExplosionDamageMultiplier()
{
	return ExplosionDamageMultiplier;
}

float UEntityStats::GetExplosionSizeMultiplier()
{
	return ExplosionSizeMultiplier;
}

float UEntityStats::GetAttackRangeMultiplier()
{
	return AttackRangeMultiplier;
}

int32 UEntityStats::GetProjectileCountMultiplier()
{
	return ProjectileCountMultiplier;
}
