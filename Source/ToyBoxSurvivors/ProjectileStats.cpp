// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileStats.h"


float UProjectileStats::GetFireRate() const
{
	return FireRate;
}


float UProjectileStats::GetDamageAmount() const
{
	return DamageAmount;
}

float UProjectileStats::GetProjectileSpeed() const
{
	return ProjectileSpeed;
}

float UProjectileStats::GetProjectileLifetime() const
{
	return ProjectileLifetime;
}

int32 UProjectileStats::GetProjectileCount() const
{
	return ProjectileCount;
}

float UProjectileStats::GetProjectileScale() const
{
	return ProjectileScale;
}

float UProjectileStats::GetExplosionDamageAmount() const
{
	return ExplosionDamageAmount;
}

float UProjectileStats::GetExplosionSize() const
{
	return ExplosionSize;	
}

float UProjectileStats::GetKnockbackAmount() const
{
	return KnockbackAmount;
}

int32 UProjectileStats::GetProjectilePenetrations() const
{
	return ProjectilePenetrations;
}

bool UProjectileStats::GetInfinitePenetrations() const
{
	return InfinitePentrations;
}