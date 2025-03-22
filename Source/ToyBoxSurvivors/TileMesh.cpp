// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMesh.h"

#include "PoolableComponent.h"

ATileMesh::ATileMesh()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Poolable = CreateDefaultSubobject<UPoolableComponent>("PoolableComponent");
}

void ATileMesh::BeginPlay()
{
	Super::BeginPlay();
	Poolable->OnGetFromPool.AddDynamic(this, &ATileMesh::OnGetFromPool);
}

void ATileMesh::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Poolable->OnGetFromPool.IsAlreadyBound(this, &ATileMesh::OnGetFromPool))
		Poolable->OnGetFromPool.RemoveDynamic(this, &ATileMesh::OnGetFromPool);
}

void ATileMesh::OnGetFromPool()
{
	t = 0;
	TargetLocation = GetActorLocation();
	TargetRotation = GetActorRotation();
	OffsetLocation = TargetLocation + LocationOffset;
	FRotator rotationOffset = FRotator(0, 0, 0);
	int32 pitch = FMath::RandRange(MinRotationOffset, MaxRotationOffset);
	pitch = pitch % 2 == 0 ? -pitch : pitch;
	int32 yaw = FMath::RandRange(MinRotationOffset, MaxRotationOffset);
	yaw = yaw % 2 == 0 ? -yaw : yaw;
	int32 roll = FMath::RandRange(MinRotationOffset, MaxRotationOffset);
	roll = roll % 2 == 0 ? -roll : roll;
	rotationOffset.Pitch = pitch;
	rotationOffset.Yaw = yaw;
	rotationOffset.Roll = roll;
	OffsetRotation = TargetRotation + rotationOffset;
	SetActorTickEnabled(true);
}

void ATileMesh::RefreshVisuals()
{
	if (IsActorTickEnabled())
		return;
	
	t = 0;
	SetActorTickEnabled(true);
}


void ATileMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	t += DeltaTime / TimeToComplete;

	if (t > 1.0f)
	{
		t = 1.0f;
		SetActorTickEnabled(false);
	}

	SetActorRotation(FMath::Lerp(OffsetRotation, TargetRotation, t));
	SetActorLocation(FMath::Lerp(OffsetLocation, TargetLocation, t));
}
