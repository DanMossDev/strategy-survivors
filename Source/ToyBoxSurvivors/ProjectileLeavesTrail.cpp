// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileLeavesTrail.h"

#include "Tile.h"

UProjectileLeavesTrail::UProjectileLeavesTrail()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UProjectileLeavesTrail::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectileLeavesTrail::Init(EElementalType ElementToSet, FCollisionShape Capsule)
{
	Element = ElementToSet;
	CollisionShape = Capsule;
}


void UProjectileLeavesTrail::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FOverlapResult> OverlappingActors;
	FVector actorPos = GetOwner()->GetActorLocation();
	FCollisionQueryParams QueryParams;

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			ATile* tile = Cast<ATile>(actor);
			if (actor && tile)
			{
				tile->SetElement(Element);
			}
		}
	}
}

