// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressionManager.h"
#include "StatsManager.h"

#include "EventDispatcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatChange, EStatsType, StatChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMilestoneUnlocked, ENonStatMilestones, Milestone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddToStat, EStatsType, Stat, float, Value);

UCLASS()
class TOONTANKS_API UEventDispatcher : public UObject
{
	GENERATED_BODY()
	
public:
	static FStatChange OnStatChanged;
	static FMilestoneUnlocked OnMilestoneUnlocked;
	static FAddToStat OnAddToStat;

	static void IncomingStatChange(const EStatsType Type);
	static void IncomingMilestoneUnlocked(const ENonStatMilestones Milestone);
	static void AddToStat(const EStatsType Stat, float Value);
};
