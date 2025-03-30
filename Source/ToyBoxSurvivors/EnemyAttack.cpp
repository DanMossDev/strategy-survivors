// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttack.h"

#include "Enemy.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyAttack::UEnemyAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyAttack::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetOwner());
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MovementComponent = Enemy->MovementComponent;
}
