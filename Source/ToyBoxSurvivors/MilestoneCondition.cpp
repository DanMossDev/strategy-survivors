// Fill out your copyright notice in the Description page of Project Settings.


#include "MilestoneCondition.h"

#include "SurvivorGameInstance.h"

UMilestoneCondition::UMilestoneCondition()
{
	StatRequirements = TArray<FStatRequirement>();
	MilestoneRequirements = TArray<UMilestone*>();
}

bool UMilestoneCondition::ConditionsMet(USurvivorGameInstance* GameInstance)
{
	for (auto stats : StatRequirements)
	{
		if (GameInstance->GetStatsManager()->GetStat(stats.RequiredStat, stats.RequiredDomain) < stats.RequiredAmount)
			return false;
	}

	for (auto milestone : MilestoneRequirements)
	{
		if (!milestone->IsUnlocked)
			return false;
	}
	
	return true;
}
