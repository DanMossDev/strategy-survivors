// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsManager.h"

#include "EventDispatcher.h"
#include "FireDamage.h"
#include "IceDamage.h"
#include "MilestoneCondition.h"
#include "OilDamage.h"
#include "PhysicalDamage.h"
#include "SaveFile.h"
#include "SurvivorGameInstance.h"
#include "WaterDamage.h"

FAddToStat UEventDispatcher::OnAddToStat;
FEnemyReceivedDamage UEventDispatcher::OnEnemyReceivedDamage;

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
	Stats.Add(EStatsDomain::CharacterSpecific, NewObject<UStatsData>());

	LoadSaveData(GameInstance->GetSaveFile());

	UEventDispatcher::OnAddToStat.AddDynamic(this, &UStatsManager::AddStat);
	UEventDispatcher::OnEnemyReceivedDamage.AddDynamic(this, &UStatsManager::EnemyReceivedDamage);
}

void UStatsManager::Cleanup()
{
	if (UEventDispatcher::OnAddToStat.IsAlreadyBound(this, &UStatsManager::AddStat))
		UEventDispatcher::OnAddToStat.RemoveDynamic(this, &UStatsManager::AddStat);
}

void UStatsManager::BeginRun()
{
	Stats[EStatsDomain::Run] = NewObject<UStatsData>();
	Stats[EStatsDomain::CharacterSpecific]->LoadSaveData(GameInstance->GetCharacterStats(GameInstance->CurrentPlayerCharacter));
}

void UStatsManager::EndRun()
{
	GameInstance->SaveCharacterStats(Stats[EStatsDomain::CharacterSpecific]);
}

void UStatsManager::LoadSaveData(USaveFile* SaveFile)
{
	Stats[EStatsDomain::Persistent]->LoadSaveData(SaveFile->PersistentStats.Stats);
}

void UStatsManager::AddStat(const EStatsType Type, const float Amount)
{
	for (auto& Stat : Stats)
		Stat.Value->AddStat(Type, Amount);
	
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

void UStatsManager::EnemyReceivedDamage(const UDamageType* DamageType, const float Amount)
{
	if (DamageType->IsA(UPhysicalDamage::StaticClass()))
		AddStat(EStatsType::PhysicalDamageDealt, Amount);
	else if (DamageType->IsA(UFireDamage::StaticClass()))
		AddStat(EStatsType::FireDamageDealt, Amount);
	else if (DamageType->IsA(UOilDamage::StaticClass()))
		AddStat(EStatsType::OilDamageDealt, Amount);
	else if (DamageType->IsA(UWaterDamage::StaticClass()))
		AddStat(EStatsType::WaterDamageDealt, Amount);
	else if (DamageType->IsA(UIceDamage::StaticClass()))
		AddStat(EStatsType::IceDamageDealt, Amount);
	else
		AddStat(EStatsType::PhysicalDamageDealt, Amount);
}