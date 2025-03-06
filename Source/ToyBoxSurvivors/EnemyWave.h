// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyWave.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOONTANKS_API UEnemyWave : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	int32 AmountToSpawnPerTick = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	TMap<TSubclassOf<AEnemy>, int32> SpawnAtStartOfWave;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	TArray<TSubclassOf<AEnemy>> Wave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	int32 XPMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Wave")
	int32 HealthDropMultiplier = 10;
};
