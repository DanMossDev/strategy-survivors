// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressionManager.h"
#include "StatsManager.h"

#include "EventDispatcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatChange, EStatsType, StatChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMilestoneUnlocked, UMilestone*, Milestone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddToStat, EStatsType, Stat, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyReceivedDamage, const UDamageType*, DamageType, float, Amount);

UCLASS()
class TOONTANKS_API UEventDispatcher : public UObject
{
	GENERATED_BODY()
	
public:
	static FStatChange OnStatChanged;
	static FMilestoneUnlocked OnMilestoneUnlocked;
	static FAddToStat OnAddToStat;
	static FEnemyReceivedDamage OnEnemyReceivedDamage;

	static void IncomingStatChange(const EStatsType Type);
	static void IncomingMilestoneUnlocked(UMilestone* Milestone);
	static void AddToStat(const EStatsType Stat, float Value);

	static void EnemyReceivedDamage(const UDamageType* DamageType, const float Amount);
};
