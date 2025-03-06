// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChargeAttack.h"

#include "Enemy.h"
#include "EnemyMovementComponent.h"

// Sets default values for this component's properties
UEnemyChargeAttack::UEnemyChargeAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyChargeAttack::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	MovementComponent = Enemy->MovementComponent;
}


// Called every frame
void UEnemyChargeAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Attacking)
	{
		ProcessAttack(DeltaTime);
		return;
	}

	CooldownRemaining -= DeltaTime;

	if (CooldownRemaining <= 0)
		CheckInRange();
}

bool UEnemyChargeAttack::CheckInRange()
{
	float distance = (Enemy->TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= AttackRange * AttackRange)
	{
		AttackTime = 0.0f;
		Attacking = true;
		MovementComponent->SetComponentTickEnabled(false);
		return true;
	}

	return false;
}

void UEnemyChargeAttack::ProcessAttack(float DeltaTime)
{
	AttackTime += DeltaTime;

	if (AttackTime < TelegraphTime)
	{
		Enemy->ApplyBounceToBaseMesh(0);
		return;
	}

	if (MovementComponent->MoveForward(DeltaTime, ChargeSpeed))
	{
		AttackTime = 100.0f;
	}

	if (AttackTime > ChargeTime + TelegraphTime)
	{
		CooldownRemaining = Cooldown;
		MovementComponent->SetComponentTickEnabled(true);
		Attacking = false;
	}
}
