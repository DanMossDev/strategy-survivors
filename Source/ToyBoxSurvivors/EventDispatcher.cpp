// Fill out your copyright notice in the Description page of Project Settings.


#include "EventDispatcher.h"

FStatChange UEventDispatcher::OnStatChanged;
FMilestoneUnlocked UEventDispatcher::OnMilestoneUnlocked;

void UEventDispatcher::IncomingStatChange(const EStatsType Type)
{
	OnStatChanged.Broadcast(Type);
}

void UEventDispatcher::IncomingMilestoneUnlocked(UMilestone* Milestone)
{
	OnMilestoneUnlocked.Broadcast(Milestone);
}

void UEventDispatcher::AddToStat(const EStatsType Stat, float Value)
{
	OnAddToStat.Broadcast(Stat, Value);
}

void UEventDispatcher::EnemyReceivedDamage(const UDamageType* DamageType, const float Amount)
{
	OnEnemyReceivedDamage.Broadcast(DamageType, Amount);
}
