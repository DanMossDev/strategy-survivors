// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChargeAttack.h"

#include "AttackAnticipationIndicator.h"
#include "Enemy.h"
#include "EnemyMovementComponent.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

UEnemyChargeAttack::UEnemyChargeAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyChargeAttack::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	Enemy->OnDeathEvent.AddDynamic(this, &UEnemyChargeAttack::OnOwnerDeath);
	MovementComponent = Enemy->MovementComponent;
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UEnemyChargeAttack::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Enemy->OnDeathEvent.IsAlreadyBound(this, &UEnemyChargeAttack::OnOwnerDeath))
		Enemy->OnDeathEvent.RemoveDynamic(this, &UEnemyChargeAttack::OnOwnerDeath);
}


void UEnemyChargeAttack::Init()
{
	Attacking = false;
	CooldownRemaining = AttackTime = 0.0f;
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
		HitIndicatorScale = FVector(Enemy->GetCollisionWidth(), Enemy->GetCollisionWidth(), 0);
		AttackDistance = ChargeTime * ChargeSpeed;
		AnticipationIndicator = GameMode->GetObjectPool()->GetFromPool<AAttackAnticipationIndicator>(AnticipationIndicatorClass, Enemy->GetActorLocation(), Enemy->GetActorRotation());
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
		HitIndicatorScale.Z = AttackDistance * (AttackTime / TelegraphTime);
		FVector location = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * HitIndicatorScale.Z /2;
		//location.Z = 0.0f;
		AnticipationIndicator->UpdateAnticipation(HitIndicatorScale, location, Enemy->GetActorRotation());
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
		AnticipationIndicator->ReturnToPool();
		AnticipationIndicator = nullptr;
	}
}
