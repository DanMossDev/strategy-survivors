// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileStats.h"


float UProjectileStats::GetDamageAmount()
{
	return DamageAmount;
}

float UProjectileStats::GetProjectileSpeed()
{
	return ProjectileSpeed;
}

float UProjectileStats::GetProjectileLifetime()
{
	return ProjectileLifetime;
}

int32 UProjectileStats::GetProjectileCount()
{
	return ProjectileCount;
}

float UProjectileStats::GetProjectileScale()
{
	return ProjectileScale;
}

float UProjectileStats::GetExplosionDamageAmount()
{
	return ExplosionDamageAmount;
}

float UProjectileStats::GetExplosionSize()
{
	return ExplosionSize;	
}

float UProjectileStats::GetKnockbackAmount()
{
	return KnockbackAmount;
}

int32 UProjectileStats::GetProjectilePenetrations()
{
	return ProjectilePenetrations;
}

bool UProjectileStats::GetInfinitePenetrations()
{
	return InfinitePentrations;
}