// Fill out your copyright notice in the Description page of Project Settings.


#include "MilestoneCondition.h"

#include "SurvivorGameInstance.h"

UMilestoneCondition::UMilestoneCondition()
{
	StatRequirements = TArray<FStatRequirement>();
	MilestoneRequirements = TArray<FMilestoneRequirement>();
}


bool UMilestoneCondition::ConditionsMet(USurvivorGameInstance* GameInstance)
{
	for (auto stats : StatRequirements)
	{
		if (GameInstance->GetStatsManager()->GetStat(stats.RequiredStat, stats.RequiredDomain) < stats.RequiredAmount)
			return false;
	}

	for (auto milestones : MilestoneRequirements)
	{
		if (!GameInstance->GetProgressionManager()->AreMilestonesAchieved(milestones.Milestones))
				return false;
	}
	
	return true;
}
