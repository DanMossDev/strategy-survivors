// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "PoolableComponent.h"
#include "RotatingTurretComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ToonTanksGameMode.h"
#include "Weapon.h"

ATower::FTowerDestroyed ATower::OnTowerDestroyed;

ATower::ATower()
{
	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
	TurretComponent = CreateDefaultSubobject<URotatingTurretComponent>(TEXT("RotatingTurret"));
	TurretComponent->SetupAttachment(BaseMesh);
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);

	for (auto weapon : Weapons)
	{
		weapon->SetComponentTickEnabled(false);
	}
}

void ATower::OnGetFromPool()
{
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);

	AToonTanksGameMode::OnGameOver.AddDynamic(this, &ATower::OnGameOver);
}

void ATower::OnReturnToPool()
{
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);

	if (AToonTanksGameMode::OnGameOver.IsAlreadyBound(this, &ATower::OnGameOver))
		AToonTanksGameMode::OnGameOver.RemoveDynamic(this, &ATower::OnGameOver);
}


void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor) return;

	if (CheckDistance(TargetActor->GetActorLocation()))
	{
		TurretComponent->RotateTurret(TargetActor->GetActorLocation());
	}
}

void ATower::OnDeath()
{
	Super::OnDeath();

	OnTowerDestroyed.Broadcast();
	Destroy();
}


bool ATower::CheckDistance(const FVector& TargetLocation ) const
{
	return FVector::Dist(TargetLocation, GetActorLocation()) < AttackRange;
}

void ATower::CheckFireCondition()
{
	bool inRange = CheckDistance(TargetActor->GetActorLocation());

	for (UWeapon* weapon : Weapons)
	{
		weapon->SetComponentTickEnabled(inRange);
	}
}

void ATower::OnGameOver()
{
	SetActorTickEnabled(false);
	GetWorldTimerManager().ClearTimer(FireRateTimerHandle);
}
