// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackController.h"

#include "Enemy.h"
#include "EnemyAttack.h"

UEnemyAttackController::UEnemyAttackController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyAttackController::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());

	auto enemyComponents = Enemy->GetComponents();

	for (auto comp : enemyComponents)
	{
		if (comp->IsA(UEnemyAttack::StaticClass()))
			AttackOptions.Add(Cast<UEnemyAttack>(comp));
	}
}

void UEnemyAttackController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Enemy->IsAttacking || Enemy->CooldownRemaining > 0.0f)
		return;

	int32 i = FMath::RandRange(0, AttackOptions.Num() - 1);
	AttackOptions[i]->CheckInRange();
}

