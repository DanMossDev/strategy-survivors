// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnticipationIndicator.h"

#include "PoolableComponent.h"

AAttackAnticipationIndicator::AAttackAnticipationIndicator()
{
	Poolable = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}


void AAttackAnticipationIndicator::BeginPlay()
{
	Super::BeginPlay();

	Poolable->OnGetFromPool.AddDynamic(this, &AAttackAnticipationIndicator::OnGetFromPool);
	Poolable->OnReturnToPool.AddDynamic(this, &AAttackAnticipationIndicator::OnReturnToPool);
}

void AAttackAnticipationIndicator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Poolable->OnGetFromPool.IsAlreadyBound(this, &AAttackAnticipationIndicator::OnGetFromPool))
		Poolable->OnGetFromPool.RemoveDynamic(this, &AAttackAnticipationIndicator::OnGetFromPool);
	if (Poolable->OnReturnToPool.IsAlreadyBound(this, &AAttackAnticipationIndicator::OnReturnToPool))
		Poolable->OnReturnToPool.RemoveDynamic(this, &AAttackAnticipationIndicator::OnReturnToPool);
}

void AAttackAnticipationIndicator::UpdateAnticipation(const FVector& Scale, const FVector& OwnerLocation, const FRotator& OwnerRotation)
{
	SetActorLocation(OwnerLocation);
	SetActorRotation(OwnerRotation);

	SetActorScale3D(Scale);
}

void AAttackAnticipationIndicator::OnGetFromPool()
{
	
}

void AAttackAnticipationIndicator::OnReturnToPool()
{
	SetActorScale3D(FVector(0));
}
