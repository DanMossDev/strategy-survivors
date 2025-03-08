// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsData.h"
#include "UObject/NoExportTypes.h"
#include "StatsManager.generated.h"

enum class EMilestoneType : uint8;

UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UStatsManager : public UObject
{
	GENERATED_BODY()

public:
	UStatsManager();
	void InjectInstance(class USurvivorGameInstance* Instance);

	UStatsData* GetPersistentStats() const {return Stats[EStatsDomain::Persistent];};
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddStat(const EStatsType Type, const float Amount);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStat(const EStatsType Type, const float Amount);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetStat(const EStatsType Type, const EStatsDomain Domain) const;
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetStat(const EStatsType Type);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetAllStats();

protected:
	void LoadSaveData(class USaveFile* SaveFile);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<EStatsDomain, UStatsData*> Stats;

	USurvivorGameInstance* GameInstance;
};
