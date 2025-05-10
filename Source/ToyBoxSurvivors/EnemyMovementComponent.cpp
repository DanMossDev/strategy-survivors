// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMovementComponent.h"

#include "Enemy.h"
#include "EntityStats.h"
#include "Tank.h"

UEnemyMovementComponent::UEnemyMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	Enemy->MovementComponent = this;
}

void UEnemyMovementComponent::Init()
{
	OverrideDirection = FVector::ZeroVector;
	IsTerrain = false;
	ShouldOverrideDirection = false;
	IsRetreating = false;
}

void UEnemyMovementComponent::SetOverrideDirection(FVector Destination, bool isTerrain)
{
	OverrideDirection = Destination;
	ShouldOverrideDirection = true;
	IsTerrain = isTerrain;
}

void UEnemyMovementComponent::ClearOverrideDestination()
{
	ShouldOverrideDirection = false;
}

void UEnemyMovementComponent::BeginRetreat()
{
	IsRetreating = true;
}


void UEnemyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UEnemyMovementComponent::Move(float DeltaTime)
{
	if (!CanMove)
	{
		Enemy->ApplyBounceToBaseMesh(0);
		return;
	}

	float distance = (Enemy->TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= StoppingDistance * StoppingDistance) return;

	FVector targetLocation;
	if (ShouldOverrideDirection &&  !IsRetreating)
	{
		targetLocation = (Enemy->TargetActor->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		targetLocation += OverrideDirection * (IsTerrain ? 1.4f : 0.4f);
		targetLocation += Enemy->GetActorLocation();
	}
	else
	{
		if (!IsRetreating)
			targetLocation = Enemy->TargetActor->GetActorLocation();
		else
		{
			targetLocation = Enemy->GetActorLocation() * 2 - Enemy->TargetActor->GetActorLocation(); 
		}
	}
	
	Enemy->RotateRoot(targetLocation);
	MoveForward(DeltaTime, Enemy->EntityStats->GetMovementSpeed());
	Enemy->ApplyBounceToBaseMesh(Enemy->EntityStats->GetMovementSpeed());
}

void UEnemyMovementComponent::MoveForward(float DeltaTime, float MovementSpeed)
{
	Enemy->AddActorLocalOffset(FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f));
}