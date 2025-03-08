// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsData.h"

UStatsData::UStatsData()
{
	Stats = TMap<EStatsType, float>();
}

void UStatsData::AddStat(EStatsType Type, float Amount)
{
	if (Stats.Contains(Type))
		Stats[Type] += Amount;
	else
		Stats.Add(Type, Amount);
}

void UStatsData::SetStat(EStatsType Type, float Amount)
{
	if (Stats.Contains(Type))
		Stats[Type] = Amount;
	else
		Stats.Add(Type, Amount);
}

float UStatsData::GetStat(EStatsType Type)
{
	if (Stats.Contains(Type))
		return Stats[Type];
	
	Stats.Add(Type, 0.0f);
	return Stats[Type];
}

void UStatsData::ResetStat(EStatsType Type)
{
	if (Stats.Contains(Type))
		Stats[Type] = 0.0f;
	else
		Stats.Add(Type, 0.0f);
}

void UStatsData::ResetAllStats()
{
	for (auto& stat : Stats)
		stat.Value = 0;
}
