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
		if (RequiredCharacter != nullptr)
		{
			bool isPlayingAs = RequiredCharacter == GameInstance->CurrentPlayerCharacter;
			if (stats.RequiredDomain == EStatsDomain::Persistent)
			{
				if (isPlayingAs)
				{
					if (GameInstance->GetStatsManager()->GetStat(stats.RequiredStat, EStatsDomain::CharacterSpecific) < stats.RequiredAmount)
						return false;
				}
				else
				{
					auto characterStats = GameInstance->GetCharacterStats(RequiredCharacter);
					if (characterStats.Find(stats.RequiredStat) == nullptr || *characterStats.Find(stats.RequiredStat) < stats.RequiredAmount)
						return false;
				}
			}
			else if (RequiredCharacter != GameInstance->CurrentPlayerCharacter)
				return false;
			else
			{
				if (GameInstance->GetStatsManager()->GetStat(stats.RequiredStat, EStatsDomain::Run) < stats.RequiredAmount)
					return false;
			}
		}
		else
		{
			if (GameInstance->GetStatsManager()->GetStat(stats.RequiredStat, stats.RequiredDomain) < stats.RequiredAmount)
				return false;
		}
	}

	for (auto milestone : MilestoneRequirements)
	{
		if (!milestone->IsUnlocked)
			return false;
	}
	
	return true;
}
