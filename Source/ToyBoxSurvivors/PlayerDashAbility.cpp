// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDashAbility.h"

#include "PlayerAbilitySystem.h"
#include "PlayerHealthComponent.h"
#include "Tank.h"

void UPlayerDashAbility::InjectOwner(UPlayerAbilitySystem* Owner)
{
	Super::InjectOwner(Owner);

	Player = Cast<ATank>(OwnerSystem->GetOwner());
	PlayerHealthComponent = Player->FindComponentByClass<UPlayerHealthComponent>();
}

void UPlayerDashAbility::CastAbility()
{
	Super::CastAbility();

	PlayerHealthComponent->SetInvincibleForTime(Duration);
	Player->SnapRotationToInput();
	Player->SetOverlayColor(DashColor, true);
}

void UPlayerDashAbility::ProcessAbility(float DeltaTime, float RatioCompleted)
{
	Super::ProcessAbility(DeltaTime, RatioCompleted);

	Player->MoveForward(DashSpeed * DeltaTime);

	if (RatioCompleted > 0.5f)
		Player->SetOverlayColor(DashColor, true);
}

void UPlayerDashAbility::FinishCastingAbility()
{
	Super::FinishCastingAbility();
}
