// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRunData.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnXPChanged);

UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);

public:
	AToonTanksGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	void GameOver(bool IsWin);

	static bool _isGameOver;
	static bool IsGameOver();
	
	static FGameOver OnGameOver;
	
	class UObjectPoolComponent* GetObjectPool() const;
	
	UPROPERTY(BlueprintReadOnly)
	float RunTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentXP = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLevel = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentRequiredXP;

	UPROPERTY(BlueprintReadWrite)
	ALevelBounds* LevelBounds;
	
	void PickupXP(int32 amount);
	void PickupCoin(int32 amount);
	void PickupItem(int32 tier);
	void PickupHealth(int32 amount);

	UPROPERTY(BlueprintAssignable)
	FOnXPChanged OnXPChanged;

	UFUNCTION(BlueprintCallable)
	void SelectUnlockable(class UUnlockableData* SelectedUnlockable);
	UFUNCTION(BlueprintCallable)
	void SelectEvolveable(class UWeaponInfo* SelectedEvolveable);
	UFUNCTION(BlueprintCallable)
	void SelectItem(UWeaponInfo* SelectedWeapon);
	UFUNCTION(BlueprintCallable)
	void SelectStat(class UStatBoost* SelectedWeapon);

	class ATank* GetPlayer() const {return Player;}

	int32 GetCurrentWaveIndex();
	UEnemyWave* GetCurrentWave();

	UFUNCTION(BlueprintCallable)
	TArray<UUnlockableData*> GetRandomUnlockables(int32 TimesRerolled);

	UFUNCTION(BlueprintCallable)
	TArray<UWeaponInfo*> GetRandomEvolveable();

	UFUNCTION(BlueprintCallable)
	TArray<UUnlockableData*> GetRandomUnlockablesUncached();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelUp();

	UFUNCTION()
	void ListenForMilestoneAchieved(class UMilestone* Milestone);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMilestoneAchieved(UMilestone* Milestone);
	
	TSubclassOf<class ADamageNumber> GetDamageNumberClass() const {return DamageNumber;}
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(class UGameEndData* GameEndData);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginRun();
	
	void BeginRun();
	void SpawnEnemies(const float DeltaTime);
	void CheckLevelUp();
	void CheckForToasts();

	void NewWave();
	
	int32 TotalXP = 0;

	UPROPERTY(VisibleAnywhere, Category = "Survivor Game Mode")
	UObjectPoolComponent* ObjectPoolComponent;

	UPROPERTY(VisibleAnywhere, Category = "Survivor Game Mode")
	class ULevelStreamingManager* LevelStreamingManager;

	TArray<UWeaponInfo*> WeaponCache;
	TArray<UUnlockableData*> UnlockableCache;

	TArray<UUnlockableData*> LockedItems;

	TQueue<UMilestone*> QueuedMilestoneUnlocks;
	float MilestoneToastTimer;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	ATank* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	UGameRunData* RunData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	class UPersistentData* PersistentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	float WaveTime = 120.0f;

	UPROPERTY()
	class USurvivorGameInstance* GameInstance;

private:
	UPROPERTY()
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Mode", meta = (AllowPrivateAccess = true))
	float StartDelay = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Game Mode")
	TSubclassOf<ADamageNumber> DamageNumber;

	int32 CachedWaveIndex;

	void HandleGameStart();

public:
	virtual void Tick(float DeltaTime) override;
};
