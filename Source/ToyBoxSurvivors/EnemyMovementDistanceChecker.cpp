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

	FVector OverrideDirection;
	bool IsTerrain = false;
	if (GetClosestActorInRange(OverrideDirection, IsTerrain))
		EnemyMovementComponent->SetOverrideDirection(OverrideDirection, IsTerrain);
	else
		EnemyMovementComponent->ClearOverrideDestination();
}

bool UEnemyMovementDistanceChecker::GetClosestActorInRange(FVector& OverrideDirection, bool& IsTerrain)
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
		AvoidOtherEnemies ? ECC_GameTraceChannel8 : ECC_GameTraceChannel9,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		OverrideDirection = OverlappingActors[0].GetActor()->GetActorLocation();
		IsTerrain = OverlappingActors[0].GetActor()->ActorHasTag("Terrain");

		if (!IsTerrain)
		{
			float ClosestActorDistance = FVector::DistSquared(actorPos, OverlappingActors[0].GetActor()->GetActorLocation());
			for (int i = 1; i < OverlappingActors.Num(); i++)
			{
				float newDist = FVector::DistSquared(actorPos, OverlappingActors[i].GetActor()->GetActorLocation());
				if (newDist < ClosestActorDistance)
				{
					IsTerrain = OverlappingActors[i].GetActor()->ActorHasTag("Terrain");
					OverrideDirection = OverlappingActors[i].GetActor()->GetActorLocation();
					ClosestActorDistance = newDist;
					if (IsTerrain)
						break;
				}
			}
		}
		
		OverrideDirection = GetOwner()->GetActorLocation() - OverrideDirection;
	}
	return bHit;
}