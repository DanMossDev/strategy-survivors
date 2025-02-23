// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "BaseEntity.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AToonTanksGameMode* gameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	gameMode->GetObjectPool()->GetFromPool(Spawnable, GetActorLocation(), GetActorRotation());
	SetActorTickEnabled(false);
}

