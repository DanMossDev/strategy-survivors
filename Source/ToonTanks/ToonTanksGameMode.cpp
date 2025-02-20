// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "ObjectPoolComponent.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

bool AToonTanksGameMode::_isGameOver;
AToonTanksPlayerController* ToonTanksPlayerController;
AToonTanksGameMode::FGameOver AToonTanksGameMode::OnGameOver;

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	HandleGameStart();
}

UObjectPoolComponent* AToonTanksGameMode::GetObjectPool()
{
	return ObjectPoolComponent;
}


void AToonTanksGameMode::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));

	if (ToonTanksPlayerController)
		ToonTanksPlayerController->SetPlayerEnabledState(false);
	OnGameOver.Broadcast();
	_isGameOver = true;
	GameOver(false);
}

bool AToonTanksGameMode::IsGameOver()
{
	return _isGameOver;
}

void AToonTanksGameMode::HandleGameStart()
{
	ObjectPoolComponent = FindComponentByClass<UObjectPoolComponent>();
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	TowerCount = GetTowerCount();

	ATower::OnTowerDestroyed.AddDynamic(this, &AToonTanksGameMode::HandleTowerDied);

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
	}
}

void AToonTanksGameMode::HandleTowerDied()
{
	TowerCount--;

	if (TowerCount <= 0)
		GameOver(true);
}

int32 AToonTanksGameMode::GetTowerCount() const
{
	TArray<AActor*> towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);
	return towers.Num();
}
