// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "EnemyWave.h"
#include "ObjectPoolComponent.h"
#include "PersistentData.h"
#include "StatBoost.h"
#include "SurvivorGameInstance.h"
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

TArray<UWeaponInfo*> AToonTanksGameMode::GetRandomWeaponsCache()
{
	WeaponCache.Empty();
	auto available = TArray<UWeaponInfo*>();

	for (auto unlockable : PersistentData->Unlockables)
	{
		if (unlockable->IsA(UWeaponInfo::StaticClass()))
		{
			UWeaponInfo* weapon = Cast<UWeaponInfo>(unlockable);
			UWeapon* playersInstance = Cast<UWeapon>(Player->GetComponentByClass(weapon->WeaponComponent));
			if (weapon->IsUnlocked() && (playersInstance == nullptr || playersInstance->CanLevelUp()))
				available.Add(weapon);
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return WeaponCache;
			
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);

		WeaponCache.Add(available[rand]);
		//available.RemoveAt(rand); //Uncomment if we want no dupes
	}

	return WeaponCache;
}

TArray<UWeaponInfo*> AToonTanksGameMode::GetRandomAvailableWeapons() const
{
	auto list = TArray<UWeaponInfo*>();
	auto available = TArray<UWeaponInfo*>();

	for (auto unlockable : PersistentData->Unlockables)
	{
		if (unlockable->IsA(UWeaponInfo::StaticClass()))
		{
			UWeaponInfo* weapon = Cast<UWeaponInfo>(unlockable);
			UWeapon* playersInstance = Cast<UWeapon>(Player->GetComponentByClass(weapon->WeaponComponent));
			if (weapon->IsUnlocked() && (playersInstance == nullptr || playersInstance->CanLevelUp()))
				available.Add(weapon);
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return list;
			
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);

		list.Add(available[rand]);
		available.RemoveAt(rand);
	}

	return list;
}

TArray<UStatBoost*> AToonTanksGameMode::GetRandomAvailableStats()
{
	auto list = TArray<UStatBoost*>();
	auto available = TArray<UStatBoost*>();

	for (auto unlockable : PersistentData->Unlockables)
	{
		if (unlockable->IsA(UStatBoost::StaticClass()))
		{
			UStatBoost* statBoost = Cast<UStatBoost>(unlockable);
			if (statBoost->IsUnlocked())
				available.Add(statBoost);
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return list;
			
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);

		list.Add(available[rand]);
		available.RemoveAt(rand);
	}

	return list;
}


void AToonTanksGameMode::SelectItem(UWeaponInfo* SelectedWeapon)
{
	int32 weaponAmount = 0;
	for (auto weaponInfo : WeaponCache)
	{
		if (weaponInfo == SelectedWeapon)
			weaponAmount++;
	}
	
	UWeapon* playersInstance = Cast<UWeapon>(Player->GetComponentByClass(SelectedWeapon->WeaponComponent));
	if (playersInstance == nullptr)
	{
		UWeapon* newInstance = Cast<UWeapon>(Player->AddComponentByClass(SelectedWeapon->WeaponComponent, false, FTransform::Identity, false));
		if (weaponAmount > 1)
			newInstance->IncreaseLevel(weaponAmount - 1);
	}
	else
	{
		playersInstance->IncreaseLevel(weaponAmount);
	}
}

void AToonTanksGameMode::SelectStat(UStatBoost* SelectedStat)
{
	Player->AddStatBoost(SelectedStat);
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

void AToonTanksGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	GameInstance = Cast<USurvivorGameInstance>(GetGameInstance());
	if (GameInstance->PlayerPawn)
		DefaultPawnClass = GameInstance->PlayerPawn;
}


void AToonTanksGameMode::HandleGameStart()
{
	SetActorTickEnabled(false);
	ObjectPoolComponent = FindComponentByClass<UObjectPoolComponent>();
	
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Player = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	CurrentRequiredXP = RunData->XPRequiredForLevelUp[CurrentLevel];

	WeaponCache = TArray<UWeaponInfo*>();

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
		for (auto waveInfo : currentWave->Wave)
		{
			for (int i = 0; i < waveInfo.AmountPerTick; i++)
			{
				FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
				FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
				auto spawnedEnemy = ObjectPoolComponent->GetFromPool<AEnemy>(waveInfo.EnemyToSpawn, spawnLocation, FRotator::ZeroRotator);
				spawnedEnemy->OverrideEntityStats(waveInfo.OverrideEntityStats);
			}
		}
	}
}

int32 AToonTanksGameMode::GetCurrentWaveIndex()
{
	int32 index = FMath::Floor(RunTime / WaveTime);
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

void AToonTanksGameMode::PickupHealth(int32 amount)
{
	Player->Heal(amount);
}

void AToonTanksGameMode::CheckLevelUp()
{
	if (CurrentXP > CurrentRequiredXP)
	{
		CurrentLevel++;
		CurrentXP = CurrentXP - CurrentRequiredXP;
		CurrentRequiredXP = RunData->XPRequiredForLevelUp[CurrentLevel];
		OnLevelUp();

		CheckLevelUp();
	}
}

void AToonTanksGameMode::NewWave()
{
	if (UEnemyWave* currentWave = GetCurrentWave())
	{
		for (auto enemyInfo : currentWave->SpawnAtStartOfWave)
		{
			for (int i = 0; i < enemyInfo.AmountPerTick; i++)
			{
				FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
				FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
				auto spawnedEnemy = ObjectPoolComponent->GetFromPool<AEnemy>(enemyInfo.EnemyToSpawn, spawnLocation, FRotator::ZeroRotator);
				spawnedEnemy->OverrideEntityStats(enemyInfo.OverrideEntityStats);
			}
		}
	}
}