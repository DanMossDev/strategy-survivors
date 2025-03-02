// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

UHomingProjectile::UHomingProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHomingProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = HomingAcceleration;
}

void UHomingProjectile::Init()
{
	Lifetime = 0.0f;
}

void UHomingProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Lifetime += DeltaTime;
	
	if (Lifetime >= StartingTime)
		CheckForHoming();
}

void UHomingProjectile::CheckForHoming()
{
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(HomingRange);
	FVector actorPos = GetOwner()->GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()->GetOwner());

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		if (ProjectileMovement->HomingTargetComponent == nullptr)
			ProjectileMovement->HomingTargetComponent = OverlappingActors[0].GetComponent();

		for (int i = 0; i < OverlappingActors.Num(); i++)
		{
			if (FVector::DistSquared(actorPos, OverlappingActors[i].GetActor()->GetActorLocation()) < FVector::DistSquared(actorPos, ProjectileMovement->HomingTargetComponent->GetComponentLocation()))
				ProjectileMovement->HomingTargetComponent = OverlappingActors[i].GetComponent();
		}
	}
	else
	{
		ProjectileMovement->HomingTargetComponent = nullptr;
	}
}