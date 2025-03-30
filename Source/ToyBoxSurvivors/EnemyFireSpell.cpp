// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFireSpell.h"

#include "Enemy.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

UEnemyFireSpell::UEnemyFireSpell()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFireSpell::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyFireSpell::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Enemy->IsAttacking)
		return;
}

