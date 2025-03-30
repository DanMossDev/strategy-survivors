// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFireSpell.h"

#include "ColumnOfFire.h"
#include "Enemy.h"
#include "EnemyMovementComponent.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"

UEnemyFireSpell::UEnemyFireSpell()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFireSpell::OnOwnerDeath()
{
	
}

void UEnemyFireSpell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Enemy->IsAttacking)
		return;

	if (Enemy->CooldownRemaining <= 0)
		CheckInRange();
}

void UEnemyFireSpell::CheckInRange()
{
	float distance = (Enemy->TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= AttackRange * AttackRange)
		Enemy->BeginAttack(this);
}

void UEnemyFireSpell::BeginAttack()
{
	AttackTime = 0.0f;
	MovementComponent->SetComponentTickEnabled(false);
	switch (AttackVariant)
	{
		case 0:
			BeginAttackVariantRing();
			break;
		default:
			break;
	}
}

void UEnemyFireSpell::BeginAttackVariantRing()
{
	float angle = 360 / NumberOfFlames;
	for (int32 i = 0; i < NumberOfFlames; i++)
	{
		FVector spawnOffset = FVector(250, 0, 0);
		FRotator rotationOffset = FRotator(0, angle * i, 0);
		spawnOffset = rotationOffset.RotateVector(spawnOffset);
		
		auto column = GameMode->GetObjectPool()->GetFromPool<AColumnOfFire>(FireColumnClass, Enemy->GetActorLocation() + spawnOffset, Enemy->GetActorRotation() + rotationOffset);
		column->Init(FVector(2,2,3), 2, 1, 10); //TODO integrate into enemy stats
		column->SetOwner(GetOwner());
	}
}


void UEnemyFireSpell::ProcessAttack(float DeltaTime)
{
	AttackTime += DeltaTime;
	switch (AttackVariant)
	{
		case 0:
			ProcessAttackVariantRing(DeltaTime);
			break;
		case 1:
			ProcessAttackVariantLine(DeltaTime);
			break;
		default:
			break;
	}
}

void UEnemyFireSpell::ProcessAttackVariantRing(float DeltaTime)
{
	if (AttackTime > CastDuration)
	{
		Enemy->FinishAttack(Cooldown);
		MovementComponent->SetComponentTickEnabled(true);
	}
}

void UEnemyFireSpell::ProcessAttackVariantLine(float DeltaTime)
{
	
}
