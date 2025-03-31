// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyAttackController.h"
#include "EnemyChargeAttack.h"
#include "EnemyMovementComponent.h"
#include "EntityStats.h"
#include "EventDispatcher.h"
#include "HealthComponent.h"
#include "Pickup.h"
#include "ObjectPoolComponent.h"
#include "PoolableComponent.h"
#include "StatusEffectComponent.h"
#include "ToonTanksGameMode.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PoolableComponent = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	CreateDefaultSubobject<UEnemyAttackController>(TEXT("EnemyAttackController"));
}

void AEnemy::OnGetFromPool()
{
	StatusEffectComponent->Init();
	HealthComponent->Init(EntityStats);
	SetBaseMeshLocalTransform(MeshZeroPos, FRotator(0));
	FVector footAdjustedPosition = GetActorLocation();
	footAdjustedPosition.Z = CapsuleComponent->GetScaledCapsuleHalfHeight() + 1;
	SetActorLocation(footAdjustedPosition);

	IsAttacking = false;
	CooldownRemaining = 0.0f;

	OnEnemyGotFromPool();

	if (auto movementComponent = FindComponentByClass<UEnemyMovementComponent>())
		movementComponent->Init();
	if (auto chargeAttack = FindComponentByClass<UEnemyChargeAttack>())
		chargeAttack->Init();
}

void AEnemy::OnReturnToPool()
{
}

void AEnemy::OverrideEntityStats(UEntityStats* OverrideWith)
{
	EntityStats->OverrideWith(OverrideWith);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);

	for (auto weapon : Weapons)
	{
		weapon->SetComponentTickEnabled(false);
	}

	PoolableComponent->OnGetFromPool.AddDynamic(this, &AEnemy::OnGetFromPool);
	PoolableComponent->OnReturnToPool.AddDynamic(this, &AEnemy::OnReturnToPool);
}

void AEnemy::BeginAttack(UEnemyAttack* AttackToBegin)
{
	if (IsAttacking)
		return;
	
	Attack = AttackToBegin;
	IsAttacking = true;
	Attack->BeginAttack();
}

void AEnemy::FinishAttack(float Cooldown)
{
	IsAttacking = false;
	Attack = nullptr;
	CooldownRemaining = Cooldown;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAttacking)
	{
		CooldownRemaining -= DeltaTime;
		return;
	}

	if (Attack != nullptr)
		Attack->ProcessAttack(DeltaTime);
}

void AEnemy::OnDeath()
{
	Super::OnDeath();

	OnEnemyDeath();
	OnDeathEvent.Broadcast();

	if (Attack)
		Attack->OnOwnerDeath();

	StatusEffectComponent->ClearAllEffects();
	SpawnRandomPickup();
	UEventDispatcher::AddToStat(EStatsType::EnemiesKilled, 1.0f);
	PoolableComponent->ReturnToPool();
}

void AEnemy::SpawnRandomPickup()
{
	float roll = FMath::RandRange(0.0f, 1.0f);
	for (auto kvp : WeightedPickupPool)
	{
		if (roll <= kvp.Key)
		{
			GameMode->GetObjectPool()->GetFromPool(kvp.Value, GetActorLocation(), GetActorRotation());
		}
	}
}
