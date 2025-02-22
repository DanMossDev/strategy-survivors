// Fill out your copyright notice in the Description page of Project Settings.


#include "Puddle.h"

#include "PoolableComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APuddle::APuddle()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("Collision"));
	SetRootComponent(CapsuleCollision);

	Poolable = CreateDefaultSubobject<UPoolableComponent>("Poolable");
}

void APuddle::Init(EElementalType Type, float Scale)
{
	Element = Type;
	RefreshVisuals();
	CurrentScale = 0;
	SetActorScale3D(FVector(0));
	TargetScale = Scale;
	RemainingLifetime = Lifetime;
}


void APuddle::BeginPlay()
{
	Super::BeginPlay();
}

void APuddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentScale < TargetScale)
	{
		CurrentScale += DeltaTime * TargetScale * 2.0f;
		if (CurrentScale > TargetScale)
			CurrentScale = TargetScale;

		SetActorScale3D(FVector(CurrentScale));
	}
	FVector loc = GetActorLocation();
	if (loc.Z > 0)
	{
		loc.Z -= DeltaTime * 1000.0f;

		if (loc.Z < 0)
		{
			loc.Z = 0;
		}
		SetActorLocation(loc);
	}

	RemainingLifetime -= DeltaTime;

	if (RemainingLifetime <= 0)
		HandleDestroy();
}

void APuddle::HitByElement(EElementalType IncomingElement)
{
	if (IncomingElement == Element)
	{
		return;
	}

	if (IncomingElement == EElementalType::Fire && Element == EElementalType::Oil)
	{
		RemainingLifetime = Lifetime;
		Element = EElementalType::Fire;
		RefreshVisuals();
	}
}


void APuddle::HandleDestroy()
{
	Poolable->ReturnToPool();
}
