// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageNumber.h"

#include "PoolableComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADamageNumber::ADamageNumber()
{
	PrimaryActorTick.bCanEverTick = true;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	Poolable = CreateDefaultSubobject<UPoolableComponent>("PoolableComponent");
}

// Called when the game starts or when spawned
void ADamageNumber::BeginPlay()
{
	Super::BeginPlay();

	Poolable->OnGetFromPool.AddDynamic(this, &ADamageNumber::OnGetFromPool);
	Poolable->OnReturnToPool.AddDynamic(this, &ADamageNumber::OnReturnToPool);
}

void ADamageNumber::OnGetFromPool()
{
	TimeAlive = 0.0f;
	RandomMovement = FVector(0.0f, 0.0f, 1.0f).GetSafeNormal();
}

void ADamageNumber::Init(float DamageToSet)
{
	SetupBlueprint(DamageToSet);
}

void ADamageNumber::OnReturnToPool()
{
}

void ADamageNumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAlive += DeltaTime;
	FVector Movement = RandomMovement * FloatUpSpeed * DeltaTime;
	Movement.Z -= FloatUpSpeed * DeltaTime * TimeAlive * 3.0f;
	AddActorLocalOffset(Movement);

	if (TimeAlive >= Lifetime)
		Poolable->ReturnToPool();
}
