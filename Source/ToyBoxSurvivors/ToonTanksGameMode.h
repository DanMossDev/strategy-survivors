// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRunData.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOver);

public:
	AToonTanksGameMode();
	
	void GameOver();

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
	
	void PickupXP(int32 amount);
	void PickupCoin(int32 amount);
	void PickupItem(int32 tier);
	void PickupHealth(int32 amount);

	UFUNCTION(BlueprintCallable)
	void SelectItem(class UWeaponInfo* SelectedWeapon);
	UFUNCTION(BlueprintCallable)
	void SelectStat(class UStatBoost* SelectedWeapon);

	int32 GetCurrentWaveIndex();
	UEnemyWave* GetCurrentWave();

	UFUNCTION(BlueprintCallable)
	TArray<UWeaponInfo*> GetRandomAvailableWeapons();

	UFUNCTION(BlueprintCallable)
	TArray<UStatBoost*> GetRandomAvailableStats();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelUp();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginRun();
	
	void BeginRun();
	void SpawnEnemies();
	void CheckLevelUp();

	void NewWave();
	
	int32 TotalXP = 0;
	
	UObjectPoolComponent* ObjectPoolComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ATank* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	UGameRunData* RunData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	class UPersistentData* PersistentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	float WaveTime = 120.0f;

private:
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float StartDelay = 3.0f;

	int32 CachedWaveIndex;

	void HandleGameStart();

public:
	virtual void Tick(float DeltaTime) override;
};
