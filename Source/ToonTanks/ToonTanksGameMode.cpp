// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "EnemyWave.h"
#include "ObjectPoolComponent.h"
#include "Tank.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"

bool AToonTanksGameMode::_isGameOver;
AToonTanksPlayerController* ToonTanksPlayerController;
AToonTanksGameMode::FGameOver AToonTanksGameMode::OnGameOver;

AToonTanksGameMode::AToonTanksGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

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
	SetActorTickEnabled(false);
	ObjectPoolComponent = FindComponentByClass<UObjectPoolComponent>();
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Player = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(this, &AToonTanksGameMode::BeginRun);
		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
	}
}

void AToonTanksGameMode::BeginRun()
{
	RunTime = 0.0f;
	ToonTanksPlayerController->SetPlayerEnabledState(true);
	SetActorTickEnabled(true);
	OnBeginRun();
}

void AToonTanksGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunTime += DeltaTime;
	SpawnEnemies();
}

void AToonTanksGameMode::SpawnEnemies()
{
	FVector spawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 2000.f;

	if (UEnemyWave* currentWave = GetCurrentWave())
	{
		for (auto enemy : currentWave->Wave)
		{
			for (int i = 0; i < currentWave->AmountToSpawnPerTick; i++)
				ObjectPoolComponent->GetFromPool<AEnemy>(enemy, spawnLocation, FRotator::ZeroRotator);
		}
	}
}

int32 AToonTanksGameMode::GetCurrentWaveIndex()
{
	return FMath::Floor(RunTime / 60.0f);
}

UEnemyWave* AToonTanksGameMode::GetCurrentWave()
{
	return RunData->WaveData.FindRef(GetCurrentWaveIndex());
}

void AToonTanksGameMode::PickupXP(int32 amount)
{
	CurrentXP += amount;
	CheckLevelUp();
}

void AToonTanksGameMode::PickupCoin(int32 amount)
{
	
}

void AToonTanksGameMode::PickupItem(int32 tier)
{
	
}

void AToonTanksGameMode::CheckLevelUp()
{
	if (CurrentXP > CurrentRequiredXP)
	{
		CurrentLevel++;
		CurrentXP = CurrentXP - CurrentRequiredXP;
		CurrentRequiredXP =  FMath::Pow(10.0f, CurrentLevel);

		CheckLevelUp();
	}
}
