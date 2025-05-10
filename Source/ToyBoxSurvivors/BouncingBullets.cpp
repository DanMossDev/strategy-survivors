// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncingBullets.h"

#include "GameFramework/ProjectileMovementComponent.h"

UBouncingBullets::UBouncingBullets()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBouncingBullets::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
}

void UBouncingBullets::Init()
{
	QueryParams.ClearIgnoredSourceObjects();
}

void UBouncingBullets::OnHitEnemy(const AActor* EnemyHit)
{
	QueryParams.AddIgnoredActor(EnemyHit);
	AcquireNewTarget();
}

void UBouncingBullets::AcquireNewTarget()
{
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(DistanceToCheckForNewTarget);
	FVector actorPos = GetOwner()->GetActorLocation();

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
		ProjectileMovement->Velocity = (OverlappingActors[FMath::RandRange(0, OverlappingActors.Num() - 1)].GetActor()->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal() * ProjectileMovement->Velocity.Length();
	}
}
