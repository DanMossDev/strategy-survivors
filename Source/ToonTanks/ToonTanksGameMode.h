// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void GameOver();

	static bool _isGameOver;
	static bool IsGameOver();
	
	static FGameOver OnGameOver;
	
	class UObjectPoolComponent* GetObjectPool();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UObjectPoolComponent* ObjectPoolComponent;

private:
	class AToonTanksPlayerController* ToonTanksPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float StartDelay = 3.0f;

	void HandleGameStart();
	UFUNCTION()
	void HandleTowerDied();

	int32 TowerCount = 0;
	int32 GetTowerCount() const;
};
