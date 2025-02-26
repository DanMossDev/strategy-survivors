// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitateToPlayerInRange.h"

#include "EntityStats.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

UGravitateToPlayerInRange::UGravitateToPlayerInRange()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravitateToPlayerInRange::Init()
{
	IsBeingAttracted = false;
	WasBeingAttractedLastFrame = false;
	LerpRatio = 0.0f;
	SetComponentTickInterval(0.3f);
}

void UGravitateToPlayerInRange::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void UGravitateToPlayerInRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsBeingAttracted)
	{
		if (WasBeingAttractedLastFrame)
			MoveTowardsPlayer(DeltaTime);
		else
			WasBeingAttractedLastFrame = true;
		
		return;
	}
	IsBeingAttracted = CheckPlayerInRange();
}

bool UGravitateToPlayerInRange::CheckPlayerInRange()
{
	bool inRange = (PlayerTank->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength() < FMath::Pow(PlayerTank->EntityStats->GetPickupRange(), 2);

	if (inRange)
	{
		SetComponentTickInterval(0);
	}
	return inRange;
}

void UGravitateToPlayerInRange::MoveTowardsPlayer(float DeltaTime)
{
	LerpRatio += DeltaTime;
	FVector newLocation = FMath::Lerp(GetOwner()->GetActorLocation(), PlayerTank->GetActorLocation(), LerpRatio);
	GetOwner()->SetActorLocation(newLocation);
}