// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyWave.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWaveInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveInfo")
	TSubclassOf<AEnemy> EnemyToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveInfo")
	int32 AmountPerWave = 1;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "WaveInfo")
	UEntityStats* OverrideEntityStats;
};


UCLASS(Blueprintable)
class TOONTANKS_API UEnemyWave : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	TArray<FEnemyWaveInfo> SpawnAtStartOfWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	TArray<FEnemyWaveInfo> Wave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	int32 XPMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	int32 HealthDropMultiplier = 10;

	UPROPERTY()
	TArray<float> AmountToSpawnThisTick;
};
