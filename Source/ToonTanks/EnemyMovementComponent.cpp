// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMovementComponent.h"

#include "Enemy.h"
#include "EntityStats.h"

// Sets default values for this component's properties
UEnemyMovementComponent::UEnemyMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	Enemy->MovementComponent = this;
}


// Called every frame
void UEnemyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UEnemyMovementComponent::Move(float DeltaTime)
{
	if (!CanMove) return;

	float distance = (Enemy->TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= StoppingDistance * StoppingDistance) return;
	
	Enemy->RotateRoot(Enemy->TargetActor->GetActorLocation());
	MoveForward(DeltaTime, Enemy->EntityStats->MovementSpeed);
}

void UEnemyMovementComponent::MoveForward(float DeltaTime, float MovementSpeed)
{
	FHitResult Hit;
	Enemy->AddActorLocalOffset(FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f), true, &Hit);

	if (Hit.bBlockingHit)
	{
	}
}
