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
	
	class UObjectPoolComponent* GetObjectPool();

	void PickupXP(int32 amount);
	void PickupCoin(int32 amount);
	void PickupItem(int32 tier);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	int32 GetCurrentWaveIndex();
	UEnemyWave* GetCurrentWave();

	void BeginRun();
	void SpawnEnemies();

	float RunTime = 0.0f;

	int32 CurrentXP = 0;
	int32 CurrentLevel = 1;

	UObjectPoolComponent* ObjectPoolComponent;
	class ATank* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	UGameRunData* RunData;

private:
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float StartDelay = 3.0f;

	void HandleGameStart();

public:
	virtual void Tick(float DeltaTime) override;
};
