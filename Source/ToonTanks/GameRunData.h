// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyWave.h"
#include "Engine/DataAsset.h"
#include "GameRunData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOONTANKS_API UGameRunData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Run Data")
	TArray<UEnemyWave*> WaveData;
};
