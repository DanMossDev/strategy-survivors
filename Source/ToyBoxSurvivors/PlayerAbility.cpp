// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbility.h"

#include "PlayerAbilitySystem.h"

void UPlayerAbility::InjectOwner(UPlayerAbilitySystem* Owner)
{
	OwnerSystem = Owner;
}

void UPlayerAbility::UpdateAbility(float DeltaTime)
{
	if (DurationRemaining > 0.0f)
	{
		DurationRemaining -= DeltaTime;
		ProcessAbility(DeltaTime, DurationRemaining / Duration);

		if (DurationRemaining <= 0.0f)
		{
			DurationRemaining = 0.0f;
			FinishCastingAbility();
		}
	}
	else if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining -= DeltaTime;

		if (CooldownRemaining < 0.0f)
			CooldownRemaining = 0.0f;
	}
}

bool UPlayerAbility::TryCastAbility()
{
	if (CooldownRemaining <= 0.0f)
	{
		CastAbility();
		return true;
	}
	return false;
}

void UPlayerAbility::CastAbility()
{
	if (Duration > 0.0f)
		DurationRemaining = Duration;
	else
		FinishCastingAbility();
}

void UPlayerAbility::FinishCastingAbility()
{
	CooldownRemaining = Cooldown;

	if (IsGlobalCooldown)
		OwnerSystem->SetGlobalCooldown(Cooldown);
}

bool UPlayerAbility::IsMovementLocked() const
{
	if (!BlockMovementDuringUse || DurationRemaining <= 0.0f)
		return false;

	return true;
}

bool UPlayerAbility::IsShootingLocked() const
{
	if (!BlockShootingDuringUse || DurationRemaining <= 0.0f)
		return false;

	return true;
}

bool UPlayerAbility::IsCastingLocked() const
{
	if (!BlockCastingDuringUse || DurationRemaining <= 0.0f)
		return false;

	return true;
}