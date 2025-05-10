// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBouncingBulletsAbility.h"

#include "PlayerAbilitySystem.h"
#include "Tank.h"

void UPlayerBouncingBulletsAbility::CastAbility()
{
	Super::CastAbility();

	OwnerSystem->GetPlayer()->SetBouncingBulletsEnabled(true);
}

void UPlayerBouncingBulletsAbility::FinishCastingAbility()
{
	Super::FinishCastingAbility();

	OwnerSystem->GetPlayer()->SetBouncingBulletsEnabled(false);
}
