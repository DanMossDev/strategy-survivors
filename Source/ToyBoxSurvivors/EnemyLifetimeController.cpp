// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLifetimeController.h"

#include "Enemy.h"
#include "EnemyMovementComponent.h"
#include "PoolableComponent.h"

UEnemyLifetimeController::UEnemyLifetimeController()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetComponentTickInterval(5);
}

void UEnemyLifetimeController::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	EnemyMovementComponent = GetOwner()->FindComponentByClass<UEnemyMovementComponent>();
	PoolableComponent = GetOwner()->FindComponentByClass<UPoolableComponent>();

	PoolableComponent->OnGetFromPool.AddDynamic(this, &UEnemyLifetimeController::OnGetFromPool);
}

void UEnemyLifetimeController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (PoolableComponent->OnGetFromPool.IsAlreadyBound(this, &UEnemyLifetimeController::OnGetFromPool))
		PoolableComponent->OnGetFromPool.RemoveDynamic(this, &UEnemyLifetimeController::OnGetFromPool);
}

void UEnemyLifetimeController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeAlive += DeltaTime;

	if (!RetreatStarted && TimeAlive > 60.0f)
	{
		if (EnemyMovementComponent)
		{
			EnemyMovementComponent->BeginRetreat();
		}
		RetreatStarted = true;
	}
	else if (RetreatStarted)
	{
		if (!Enemy->IsVisible())
			PoolableComponent->ReturnToPool();
	}
	if (TimeAlive > 90.0f)
		PoolableComponent->ReturnToPool();
}

void UEnemyLifetimeController::OnGetFromPool()
{
	TimeAlive = 0.0f;
	RetreatStarted = false;
}
