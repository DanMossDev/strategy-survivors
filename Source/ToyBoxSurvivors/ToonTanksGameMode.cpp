// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "EnemyWave.h"
#include "EventDispatcher.h"
#include "GameEndData.h"
#include "Inventory.h"
#include "LevelStreamingManager.h"
#include "ObjectPoolComponent.h"
#include "PersistentData.h"
#include "PlayableCharacter.h"
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

	ObjectPoolComponent = CreateDefaultSubobject<UObjectPoolComponent>("ObjectPoolComponent");
	LevelStreamingManager = CreateDefaultSubobject<ULevelStreamingManager>("LevelStreamingManager");
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

TArray<UWeaponInfo*> AToonTanksGameMode::GetRandomEvolveable()
{
	auto list = TArray<UWeaponInfo*>();
	auto available = Player->GetInventory()->GetEvolveables();

	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return list;
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);
		list.Add(available[rand]);
	}

	return list;
}


TArray<UUnlockableData*> AToonTanksGameMode::GetRandomUnlockables(int32 TimesRerolled)
{
	UnlockableCache.Empty();
	auto available = TArray<UUnlockableData*>();
	
	for (auto unlockable : PersistentData->Unlockables)
	{
		if (unlockable->IsA(UPlayableCharacter::StaticClass()))
			continue;
		
		if (unlockable->IsA(UWeaponInfo::StaticClass()))
		{
			if (!Player->GetInventory()->CanFindNewWeapons())
				continue;
		}
		else if (!Player->GetInventory()->CanFindNewStats())
			continue;

		if (Player->GetInventory()->IsUnlockableMaxed(unlockable))
			continue;
		
		if (unlockable->IsUnlocked())
			available.Add(unlockable);
	}

	available.Append(Player->GetInventory()->GetUpgradeables());
	
	for (int i = 0; i < 3 - TimesRerolled; i++)
	{
		if (available.Num() == 0)
			return UnlockableCache;
			
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);
		UnlockableCache.Add(available[rand]);
	}

	return UnlockableCache;
}

TArray<UUnlockableData*> AToonTanksGameMode::GetRandomUnlockablesUncached()
{
	auto list = TArray<UUnlockableData*>();
	auto available = TArray<UUnlockableData*>();
	
	for (auto unlockable : PersistentData->Unlockables)
	{
		if (unlockable->IsA(UPlayableCharacter::StaticClass()))
			continue;
		
		if (unlockable->IsA(UWeaponInfo::StaticClass()))
		{
			if (!Player->GetInventory()->CanFindNewWeapons())
				continue;
		}
		else if (!Player->GetInventory()->CanFindNewStats())
			continue;

		if (Player->GetInventory()->IsUnlockableMaxed(unlockable))
			continue;
		
		if (unlockable->IsUnlocked())
			available.Add(unlockable);
	}

	available.Append(Player->GetInventory()->GetUpgradeables());
	
	for (int i = 0; i < 3; i++)
	{
		if (available.Num() == 0)
			return list;
		
		int32 rand = FMath::RandRange(0, available.Num() - 1);
		list.Add(available[rand]);
	}

	return list;
}

void AToonTanksGameMode::SelectEvolveable(UWeaponInfo* SelectedEvolveable)
{
	UWeapon* playersInstance = Cast<UWeapon>(Player->GetComponentByClass(SelectedEvolveable->WeaponComponent));
	
	if (playersInstance)
	{
		playersInstance->Evolve();
		Player->GetInventory()->EvolveWeapon(SelectedEvolveable);
	}

	for (auto requirement : SelectedEvolveable->EvolutionRequirements)
	{
		if (requirement->IsA(UWeaponInfo::StaticClass()))
		{
			UWeaponInfo* mergedWeapon = Cast<UWeaponInfo>(requirement);
			UWeapon* playersInstanceOfRequirement = Cast<UWeapon>(Player->GetComponentByClass(mergedWeapon->WeaponComponent));

			playersInstanceOfRequirement->DestroyComponent();
			Player->GetInventory()->WeaponMerged(mergedWeapon);
		}
	}
}

void AToonTanksGameMode::SelectUnlockable(UUnlockableData* SelectedUnlockable)
{
	if (SelectedUnlockable->IsA(UWeaponInfo::StaticClass()))
	{
		SelectItem(Cast<UWeaponInfo>(SelectedUnlockable));
	}
	else
	{
		SelectStat(Cast<UStatBoost>(SelectedUnlockable));
	}
}

void AToonTanksGameMode::SelectItem(UWeaponInfo* SelectedWeapon)
{
	int32 weaponAmount = 0;
	
	if (UnlockableCache.Num() == 0)
		weaponAmount = 1;
	
	for (auto weaponInfo : UnlockableCache)
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

	Player->GetInventory()->AddToInventory(SelectedWeapon, weaponAmount);
}

void AToonTanksGameMode::SelectStat(UStatBoost* SelectedStat)
{
	int32 statLevel = 0;
	
	if (UnlockableCache.Num() == 0)
		statLevel = 1;

	for (auto statBoost : UnlockableCache)
	{
		if (statBoost == SelectedStat)
			statLevel++;
	}
	
	int32 amountAdded = Player->GetInventory()->AddToInventory(SelectedStat, statLevel);

	for (int i = 0; i < amountAdded; i++)
	{
		Player->AddStatBoost(SelectedStat);
	}
}

void AToonTanksGameMode::GameOver(bool IsWin)
{
	if (UEventDispatcher::OnMilestoneUnlocked.IsAlreadyBound(this, &AToonTanksGameMode::ListenForMilestoneAchieved))
		UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &AToonTanksGameMode::ListenForMilestoneAchieved);
	
	if (ToonTanksPlayerController)
		ToonTanksPlayerController->SetPlayerEnabledState(false);

	UEventDispatcher::AddToStat(IsWin ? EStatsType::TotalRunsWon : EStatsType::TotalRunsLost, 1);
	GameInstance->GetStatsManager()->EndRun();
	OnGameOver.Broadcast();
	_isGameOver = true;

	GameInstance->SaveGame();

	TArray<UUnlockableData*> unlockedThisRun = TArray<UUnlockableData*>();

	for (auto item : LockedItems)
		if (item->IsUnlocked())
			unlockedThisRun.Add(item);
	
	UGameEndData* GameEndData = NewObject<UGameEndData>();
	GameEndData->IsWin = IsWin;
	GameEndData->EnemiesKilled = GameInstance->GetStatsManager()->GetStat(EStatsType::EnemiesKilled, EStatsDomain::Run);
	GameEndData->DistanceTravelled = GameInstance->GetStatsManager()->GetStat(EStatsType::DistanceTravelled, EStatsDomain::Run);
	GameEndData->TimeSurvived = RunTime;
	GameEndData->UnlockedThisRun = unlockedThisRun;
	GameOver(GameEndData);
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
	
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	Player = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	LevelStreamingManager->Initialize();

	CurrentRequiredXP = RunData->XPRequiredForLevelUp[CurrentLevel];

	WeaponCache = TArray<UWeaponInfo*>();

	LockedItems = TArray<UUnlockableData*>();

	for (auto item : PersistentData->Unlockables)
		if (!item->IsUnlocked())
			LockedItems.Add(item);

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
	GameInstance->GetStatsManager()->BeginRun();
	ToonTanksPlayerController->SetPlayerEnabledState(true);
	SetActorTickEnabled(true);
	OnBeginRun();

	UEventDispatcher::OnMilestoneUnlocked.AddDynamic(this, &AToonTanksGameMode::ListenForMilestoneAchieved);
}

void AToonTanksGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RunTime += DeltaTime;
	UEventDispatcher::AddToStat(EStatsType::TimeSurvived, DeltaTime);

	MilestoneToastTimer -= DeltaTime;
	if (MilestoneToastTimer <= 0)
		CheckForToasts();
	SpawnEnemies(DeltaTime);
}

void AToonTanksGameMode::SpawnEnemies(const float DeltaTime)
{
	if (RunData->WaveData.Num() > GetCurrentWaveIndex())
	{
		if (UEnemyWave* currentWave = GetCurrentWave())
		{
			for (int i = 0; i < currentWave->Wave.Num(); i++)
			{
				auto waveInfo = currentWave->Wave[i];

				float gainPerSecond = waveInfo.AmountPerWave / WaveTime;
				currentWave->AmountToSpawnThisTick[i] += DeltaTime * gainPerSecond;

				while (currentWave->AmountToSpawnThisTick[i] > 1)
				{
					FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
					FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
					auto spawnedEnemy = ObjectPoolComponent->GetFromPool<AEnemy>(waveInfo.EnemyToSpawn, spawnLocation, FRotator::ZeroRotator);
					spawnedEnemy->OverrideEntityStats(waveInfo.OverrideEntityStats);
					
					currentWave->AmountToSpawnThisTick[i]--;
				}
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
	OnXPChanged.Broadcast();
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
	if (CurrentXP >= CurrentRequiredXP)
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
	if (RunData->WaveData.Num() > GetCurrentWaveIndex())
	{
		if (UEnemyWave* currentWave = GetCurrentWave())
		{
			for (auto waveInfo : currentWave->Wave)
			{
				currentWave->AmountToSpawnThisTick.Add(0.0f);
			}
			for (auto enemyInfo : currentWave->SpawnAtStartOfWave)
			{
				for (int i = 0; i < enemyInfo.AmountPerWave; i++)
				{
					FVector randomForward = Player->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-60.0f, 60.0f), FVector::UpVector);
					FVector spawnLocation = Player->GetActorLocation() + randomForward * 3000.f;
					auto spawnedEnemy = ObjectPoolComponent->GetFromPool<AEnemy>(enemyInfo.EnemyToSpawn, spawnLocation, FRotator::ZeroRotator);
					spawnedEnemy->OverrideEntityStats(enemyInfo.OverrideEntityStats);
				}
			}
		}
	}
	else
	{
		GameOver(true);
	}
}

void AToonTanksGameMode::ListenForMilestoneAchieved(UMilestone* Milestone)
{
	QueuedMilestoneUnlocks.Enqueue(Milestone);
}

void AToonTanksGameMode::CheckForToasts()
{
	if (QueuedMilestoneUnlocks.IsEmpty())
		return;

	MilestoneToastTimer = 3.0f;
	UMilestone* toastableMilestone;
	QueuedMilestoneUnlocks.Dequeue(toastableMilestone);

	if (toastableMilestone)
		OnMilestoneAchieved(toastableMilestone);
}
