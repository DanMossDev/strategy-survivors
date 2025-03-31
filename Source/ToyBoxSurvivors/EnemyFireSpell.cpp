// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFireSpell.h"

#include "ColumnOfFire.h"
#include "Enemy.h"
#include "EnemyMovementComponent.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"

UEnemyFireSpell::UEnemyFireSpell()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyFireSpell::OnOwnerDeath()
{
	
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
		FVector spawnOffset = FVector(Enemy->GetCollisionWidth() + LocationOffset, 0, 0);
		FRotator rotationOffset = FRotator(0, angle * i, 0);
		spawnOffset = rotationOffset.RotateVector(spawnOffset);
		
		auto column = GameMode->GetObjectPool()->GetFromPool<AColumnOfFire>(FireColumnClass, Enemy->GetActorLocation() + spawnOffset, Enemy->GetActorRotation() + rotationOffset);
		column->Init(ColumnSize, TelegraphTime, FlameDuration, Enemy->GetSpellDamage() * DamageMultiplier);
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
	if (AttackTime > TelegraphTime + FlameDuration + 0.25f)
	{
		Enemy->FinishAttack(Cooldown);
		MovementComponent->SetComponentTickEnabled(true);
	}
}

void UEnemyFireSpell::ProcessAttackVariantLine(float DeltaTime)
{
	
}
