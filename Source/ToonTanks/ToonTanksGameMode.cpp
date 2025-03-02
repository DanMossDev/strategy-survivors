// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "EnemyWave.h"
#include "ObjectPoolComponent.h"
#include "Tank.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponInfo.h"

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

UObjectPoolComponent* AToonTanksGameMode::GetObjectPool() const
{
	return ObjectPoolComponent;
}

TArray<UWeaponInfo*> AToonTanksGameMode::GetRandomAvailableWeapons()
{
	auto list = TArray<UWeaponInfo*>();
	auto available = AvailableWeapons;
	
	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return list;
			
		
		FRandomStream random;
		int32 rand = random.RandRange(0, available.Num());

		list.Add(available[rand]);
		available.RemoveAt(rand);
	}

	return list;
}

void AToonTanksGameMode::SelectItem(UWeaponInfo* SelectedWeapon)
{
	if (AvailableWeapons.Contains(SelectedWeapon))
		AvailableWeapons.Remove(SelectedWeapon);

	Player->AddComponentByClass(SelectedWeapon->WeaponComponent, false, FTransform::Identity, false);
}

void AToonTanksGameMode::GameOver()
{
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

	for (UWeaponInfo* weapon : AllWeapons)
		if (weapon->IsUnlocked)
			AvailableWeapons.Add(weapon);
}

void AToonTanksGameMode::BeginRun()
{
	RunTime = 0.0f;
	CachedWaveIndex = -1;
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
	if (UEnemyWave* currentWave = GetCurrentWave())
	{
		for (auto enemy : currentWave->Wave)
		{
			for (int i = 0; i < currentWave->AmountToSpawnPerTick; i++)
			{
				FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
				FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
				ObjectPoolComponent->GetFromPool<AEnemy>(enemy, spawnLocation, FRotator::ZeroRotator);
			}
		}
	}
}

int32 AToonTanksGameMode::GetCurrentWaveIndex()
{
	int32 index = FMath::Floor(RunTime / 60.0f);
	if (index > CachedWaveIndex)
	{
		CachedWaveIndex = index;
		NewWave();
	}
	return index;
}

UEnemyWave* AToonTanksGameMode::GetCurrentWave()
{
	return RunData->WaveData[GetCurrentWaveIndex()];
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

void AToonTanksGameMode::NewWave()
{
	if (UEnemyWave* currentWave = GetCurrentWave())
	{
		for (auto enemyInfo : currentWave->SpawnAtStartOfWave)
		{
			for (int i = 0; i < enemyInfo.Value; i++)
			{
				FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
				FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
				ObjectPoolComponent->GetFromPool<AEnemy>(enemyInfo.Key, spawnLocation, FRotator::ZeroRotator);
			}
		}
	}
}
