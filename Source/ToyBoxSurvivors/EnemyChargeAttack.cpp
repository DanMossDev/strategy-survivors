// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChargeAttack.h"

#include "AttackAnticipationIndicator.h"
#include "Enemy.h"
#include "EnemyMovementComponent.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"

UEnemyChargeAttack::UEnemyChargeAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyChargeAttack::Init()
{
	AttackTime = 0.0f;
}

void UEnemyChargeAttack::OnOwnerDeath()
{
	if (AnticipationIndicator != nullptr)
	{
		AnticipationIndicator->ReturnToPool();
		AnticipationIndicator = nullptr;
	}
}


void UEnemyChargeAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Enemy->IsAttacking)
		return;

	if (Enemy->CooldownRemaining <= 0)
		CheckInRange();
}

void UEnemyChargeAttack::CheckInRange()
{
	float distance = (Enemy->TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= AttackRange * AttackRange)
		Enemy->BeginAttack(this);
}

void UEnemyChargeAttack::BeginAttack()
{
	AttackTime = 0.0f;
	HitIndicatorScale = FVector(Enemy->GetCollisionWidth(), Enemy->GetCollisionWidth(), 0);
	AttackDistance = ChargeTime * ChargeSpeed;
	AnticipationIndicator = GameMode->GetObjectPool()->GetFromPool<AAttackAnticipationIndicator>(AnticipationIndicatorClass, Enemy->GetActorLocation(), Enemy->GetActorRotation());
	MovementComponent->SetComponentTickEnabled(false);
}

void UEnemyChargeAttack::ProcessAttack(float DeltaTime)
{
	AttackTime += DeltaTime;

	if (AttackTime < TelegraphTime)
	{
		Enemy->ChargeWindup(AttackTime / TelegraphTime);
		HitIndicatorScale.Z = AttackDistance * (AttackTime / TelegraphTime);
		FVector location = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * HitIndicatorScale.Z /2;
		location.Z = 0.0f;
		AnticipationIndicator->UpdateAnticipation(HitIndicatorScale, location, Enemy->GetActorRotation());
		return;
	}

	HitIndicatorScale.Z = AttackDistance * (1 - (AttackTime - TelegraphTime) / ChargeTime);
	FVector location = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * HitIndicatorScale.Z /2;
	location.Z = 0.0f;
	AnticipationIndicator->UpdateAnticipation(HitIndicatorScale, location, Enemy->GetActorRotation());

	if (MovementComponent->MoveForward(DeltaTime, ChargeSpeed))
	{
		AttackTime = 100.0f;
	}

	if (AttackTime > ChargeTime + TelegraphTime)
	{
		MovementComponent->SetComponentTickEnabled(true);
		AnticipationIndicator->ReturnToPool();
		AnticipationIndicator = nullptr;
		Enemy->FinishAttack(Cooldown);
	}
}
