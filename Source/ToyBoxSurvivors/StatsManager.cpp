// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsManager.h"

#include "EventDispatcher.h"
#include "MilestoneCondition.h"
#include "SaveFile.h"
#include "SurvivorGameInstance.h"

FAddToStat UEventDispatcher::OnAddToStat;

UStatsManager::UStatsManager()
{
	Stats = TMap<EStatsDomain, UStatsData*>();
	Stats.Add(EStatsDomain::Persistent, NewObject<UStatsData>());
}

void UStatsManager::InjectInstance(class USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
	
	Stats.Add(EStatsDomain::Persistent, NewObject<UStatsData>());
	Stats.Add(EStatsDomain::Run, NewObject<UStatsData>());
	Stats.Add(EStatsDomain::Objective, NewObject<UStatsData>());

	LoadSaveData(GameInstance->GetSaveFile());

	UEventDispatcher::OnAddToStat.AddDynamic(this, &UStatsManager::AddStat);
}

void UStatsManager::LoadSaveData(USaveFile* SaveFile)
{
	Stats[EStatsDomain::Persistent]->LoadSaveData(SaveFile->PersistentStats);
}

void UStatsManager::AddStat(const EStatsType Type, const float Amount)
{
	for (auto& Stat : Stats)
		Stat.Value->AddStat(Type, Amount);


	GameInstance->SaveGame();
	UEventDispatcher::IncomingStatChange(Type);
}

void UStatsManager::SetStat(const EStatsType Type, const float Amount)
{
	for (auto& Stat : Stats)
		Stat.Value->SetStat(Type, Amount);

	UEventDispatcher::IncomingStatChange(Type);
}

float UStatsManager::GetStat(const EStatsType Type, const EStatsDomain Domain) const
{
	return Stats[Domain]->GetStat(Type);
}

void UStatsManager::ResetStat(const EStatsType Type)
{
	for (auto& Stat : Stats)
		Stat.Value->ResetStat(Type);
}

void UStatsManager::ResetAllStats()
{
	for (auto& Stat : Stats)
		Stat.Value->ResetAllStats();
}