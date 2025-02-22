// Fill out your copyright notice in the Description page of Project Settings.


#include "Puddle.h"

#include "PoolableComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APuddle::APuddle()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(FName("SphereCollision"));
	SetRootComponent(SphereCollision);

	Poolable = CreateDefaultSubobject<UPoolableComponent>("Poolable");
}

void APuddle::Init()
{
	RemainingLifetime = Lifetime;
}

void APuddle::BeginPlay()
{
	Super::BeginPlay();

	Poolable->OnGetFromPool.AddDynamic(this, &APuddle::Init);
}

void APuddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemainingLifetime -= DeltaTime;

	if (RemainingLifetime <= 0)
		HandleDestroy();
}

void APuddle::HitByElement(EElementalType IncomingElement)
{
	if (IncomingElement == Element)
	{
		RemainingLifetime = Lifetime;
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
