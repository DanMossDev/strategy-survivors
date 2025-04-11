// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMovementComponent.h"

#include "Enemy.h"
#include "EntityStats.h"
#include "PhysicalDamage.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

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
	if (ShouldOverrideDirection)
	{
		FVector targetLocation = (Enemy->TargetActor->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal() * (IsTerrain ? 350.0f : 450.0f);
		targetLocation += OverrideDirection;
		Enemy->RotateRoot(Enemy->GetActorLocation() + targetLocation);
	}
	else
		Enemy->RotateRoot(Enemy->TargetActor->GetActorLocation());
	
	MoveForward(DeltaTime, Enemy->EntityStats->GetMovementSpeed());
	Enemy->ApplyBounceToBaseMesh(Enemy->EntityStats->GetMovementSpeed());
}

bool UEnemyMovementComponent::MoveForward(float DeltaTime, float MovementSpeed)
{
	FHitResult Hit;
	Enemy->AddActorLocalOffset(FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f), true, &Hit);

	if (Hit.bBlockingHit)
	{
		if (Hit.GetActor() == Enemy->TargetActor)
		{
			UGameplayStatics::ApplyDamage(Enemy->TargetActor, Enemy->EntityStats->GetContactDamageAmount(), Enemy->GetInstigatorController(), Enemy, UPhysicalDamage::StaticClass());
			Enemy->SetKnockbackAmount(Enemy->GetActorForwardVector() * -Enemy->EntityStats->GetKnockbackAmount(), 1.0f);
			ATank* player = Cast<ATank>(Enemy->TargetActor);
			if (player)
				player->SetKnockbackAmount(Enemy->GetActorForwardVector() * Enemy->EntityStats->GetKnockbackAmount(), 0.25f);
			return true;
		}
	}
	return false;
}