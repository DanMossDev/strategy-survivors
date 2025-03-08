// Fill out your copyright notice in the Description page of Project Settings.


#include "Milestone.h"

#include "EventDispatcher.h"
#include "MilestoneCondition.h"
#include "SurvivorGameInstance.h"

// FStatChange UEventDispatcher::OnStatChanged;
// FMilestoneUnlocked UEventDispatcher::OnMilestoneUnlocked;

void UMilestone::InjectInstance(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;

	if (GameInstance->GetProgressionManager()->AreMilestonesAchieved(Milestone))
		return;

	UEventDispatcher::OnStatChanged.AddDynamic(this, &UMilestone::OnStatChanged);
	UEventDispatcher::OnMilestoneUnlocked.AddDynamic(this, &UMilestone::OnMilestoneUnlocked);
}

void UMilestone::OnStatChanged(EStatsType ChangedStat)
{
	if (!MilestoneCondition->ConditionsMet(GameInstance))
		return;

	UEventDispatcher::OnStatChanged.RemoveDynamic(this, &UMilestone::OnStatChanged);
	UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &UMilestone::OnMilestoneUnlocked);
	GameInstance->GetProgressionManager()->MilestoneAchieved(Milestone);
}

void UMilestone::OnMilestoneUnlocked(EMilestoneType UnlockedMilestone)
{
	if (!MilestoneCondition->ConditionsMet(GameInstance))
		return;

	UEventDispatcher::OnStatChanged.RemoveDynamic(this, &UMilestone::OnStatChanged);
	UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &UMilestone::OnMilestoneUnlocked);
	GameInstance->GetProgressionManager()->MilestoneAchieved(Milestone);
}
