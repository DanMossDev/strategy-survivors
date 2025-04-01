// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsData.generated.h"

UENUM()
enum class EStatsType : uint8
{
	EnemiesKilled,
	DistanceTravelled,
	TotalRunsWon,
	TotalRunsLost
};

UENUM()
enum class EStatsDomain : uint8
{
	Persistent,
	Run,
	CharacterSpecific
};


UCLASS()
class TOONTANKS_API UStatsData : public UObject
{
	GENERATED_BODY()

public:
	UStatsData();

	void LoadSaveData(const TMap<EStatsType, float>& LoadedStats) {Stats = LoadedStats;}
	TMap<EStatsType, float>& GetStats() {return Stats;}
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddStat(EStatsType Type, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStat(EStatsType Type, float Amount);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetStat(EStatsType Type);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetStat(EStatsType Type);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetAllStats();

protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<EStatsType, float> Stats;
};
