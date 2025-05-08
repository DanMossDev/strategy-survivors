// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTurretModeAbility.h"

#include "PlayerAbilitySystem.h"
#include "RotatingTurretComponent.h"
#include "Tank.h"

void UPlayerTurretModeAbility::CastAbility()
{
	Super::CastAbility();

	TurretStartScale = OwnerSystem->GetPlayer()->GetTurretComponent()->GetRelativeScale3D();
	TurretStartPosition = OwnerSystem->GetPlayer()->GetTurretComponent()->GetRelativeLocation();
}

void UPlayerTurretModeAbility::ProcessAbility(float DeltaTime, float RatioCompleted)
{
	Super::ProcessAbility(DeltaTime, RatioCompleted);

	if (RatioCompleted < 0.05f)
	{
		OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeScale3D(FMath::Lerp(TurretStartScale, TurretTargetScale, RatioCompleted / 0.05f));
		OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeLocation(FMath::Lerp(TurretStartPosition, TurretTargetPosition, RatioCompleted / 0.05f));
	}
	else if (RatioCompleted > 0.95f)
	{
		OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeScale3D(FMath::Lerp(TurretStartScale, TurretTargetScale, (1 - RatioCompleted) / 0.05f));
		OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeLocation(FMath::Lerp(TurretStartPosition, TurretTargetPosition, (1 - RatioCompleted)  / 0.05f));
	}
}

void UPlayerTurretModeAbility::FinishCastingAbility()
{
	Super::FinishCastingAbility();
	
	OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeLocation(TurretStartPosition);
	OwnerSystem->GetPlayer()->GetTurretComponent()->SetRelativeScale3D(TurretStartScale);
}
