// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingTurretComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URotatingTurretComponent::URotatingTurretComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URotatingTurretComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URotatingTurretComponent::RotateTurret(FVector TargetLocation)
{
	FVector direction = TargetLocation - GetComponentLocation();
	
	if (direction.SquaredLength() < MinimumDistanceToAllowRotate * MinimumDistanceToAllowRotate) return;
	
	FRotator targetRotation = FRotator(0, direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetComponentRotation(), targetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), TurretRotationSpeed);
	SetWorldRotation(actualRotation);
}