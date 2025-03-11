// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMovementDistanceChecker.h"

#include "EnemyMovementComponent.h"
#include "Components/CapsuleComponent.h"

UEnemyMovementDistanceChecker::UEnemyMovementDistanceChecker()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyMovementDistanceChecker::BeginPlay()
{
	Super::BeginPlay();

	EnemyMovementComponent = GetOwner()->FindComponentByClass<UEnemyMovementComponent>();
	EnemyCollision = GetOwner()->FindComponentByClass<UCapsuleComponent>();
}

void UEnemyMovementDistanceChecker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector ClosestActor;
	if (GetClosestActorInRange(ClosestActor))
		EnemyMovementComponent->SetOverrideDestination(ClosestActor);
	else
		EnemyMovementComponent->ClearOverrideDestination();
}

bool UEnemyMovementDistanceChecker::GetClosestActorInRange(FVector& ClosestActor)
{
	FVector actorPos = GetOwner()->GetActorLocation();
	TArray<FOverlapResult> OverlappingActors;

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(EnemyCollision->GetScaledCapsuleRadius() + AvoidObjectsDistance);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel8,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		ClosestActor = OverlappingActors[0].GetActor()->GetActorLocation();
		float ClosestActorDistance = FVector::DistSquared(actorPos, OverlappingActors[0].GetActor()->GetActorLocation());
		for (int i = 1; i < OverlappingActors.Num(); i++)
		{
			float newDist = FVector::DistSquared(actorPos, OverlappingActors[i].GetActor()->GetActorLocation());
			if (newDist < ClosestActorDistance)
			{
				ClosestActor = OverlappingActors[i].GetActor()->GetActorLocation();
				ClosestActorDistance = newDist;
			}
		}
	}
	return bHit;
}