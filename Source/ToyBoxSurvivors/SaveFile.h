// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsManager.h"
#include "GameFramework/SaveGame.h"
#include "SaveFile.generated.h"

USTRUCT(BlueprintType)
struct FSavedStats
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStatsType, float> Stats = TMap<EStatsType, float>();
};


UCLASS()
class TOONTANKS_API USaveFile : public USaveGame
{
	GENERATED_BODY()

public:
	USaveFile();

	UPROPERTY(BlueprintReadWrite)
	float Volume;
	UPROPERTY(BlueprintReadWrite)
	float SFXVolume;
	UPROPERTY(BlueprintReadWrite)
	float MusicVolume;

	UPROPERTY(BlueprintReadWrite)
	TArray<FGuid> CompletedMilestones;

	UPROPERTY(BlueprintReadOnly)
	FSavedStats PersistentStats;
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FSavedStats> CharacterSpecificStats;
};