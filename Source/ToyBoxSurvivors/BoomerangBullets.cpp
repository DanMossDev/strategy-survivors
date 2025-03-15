// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerangBullets.h"

#include "GameFramework/ProjectileMovementComponent.h"

UBoomerangBullets::UBoomerangBullets()
{
	PrimaryComponentTick.bCanEverTick = true;

	DirectionToReturn = FVector::ZeroVector;
}

void UBoomerangBullets::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
}

void UBoomerangBullets::Init()
{
	DirectionToReturn = ProjectileMovement->Velocity.GetSafeNormal() * -1;
}

void UBoomerangBullets::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ProjectileMovement->Velocity += DirectionToReturn * BoomerangSpeed * DeltaTime;
}

