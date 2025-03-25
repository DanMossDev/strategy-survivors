// Fill out your copyright notice in the Description page of Project Settings.


#include "Milestone.h"

#include "EventDispatcher.h"
#include "MilestoneCondition.h"
#include "PersistentData.h"
#include "SurvivorGameInstance.h"

// FStatChange UEventDispatcher::OnStatChanged;
// FMilestoneUnlocked UEventDispatcher::OnMilestoneUnlocked;

void UMilestone::InjectInstance(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
	IsUnlocked = false;

	if (Instance->GetSaveFile()->CompletedMilestones.Contains(MilestoneID))
	{
		IsUnlocked = true;
		return;
	}
	
	if (MilestoneCondition && MilestoneCondition->ConditionsMet(GameInstance))
	{
		IsUnlocked = true;
		return;
	}
	
	UEventDispatcher::OnStatChanged.AddDynamic(this, &UMilestone::OnStatChanged);
}

void UMilestone::Cleanup()
{
	if (UEventDispatcher::OnStatChanged.IsAlreadyBound(this, &UMilestone::OnStatChanged))
		UEventDispatcher::OnStatChanged.RemoveDynamic(this, &UMilestone::OnStatChanged);
}

void UMilestone::OnStatChanged(EStatsType ChangedStat)
{
	if (!MilestoneCondition || !MilestoneCondition->ConditionsMet(GameInstance))
		return;
	
	IsUnlocked = true;
	Cleanup();
	GameInstance->GetProgressionManager()->MilestoneAchieved(this);
}

void UMilestone::OnMilestoneUnlocked()
{
	if (MilestoneCondition && !MilestoneCondition->ConditionsMet(GameInstance))
		return;

	IsUnlocked = true;
	Cleanup();
	GameInstance->GetProgressionManager()->MilestoneAchieved(this);
}

bool UMilestone::GetIsUnlocked() const
{
	return IsUnlocked || (!MilestoneCondition || MilestoneCondition->ConditionsMet(GameInstance));
}

#if WITH_EDITOR
void UMilestone::PostLoad()
{
	Super::PostLoad();

	if (!MilestoneID.IsValid())
		RefreshGUID();
	
	UE_LOG(LogTemp, Display, TEXT("Loading milestone"));

	auto LoadedAsset = StaticLoadObject(UPersistentData::StaticClass(), nullptr, TEXT("/Game/Data/DA_PersistentData.DA_PersistentData"));
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load asset for PersistentData"));
		return;
	}
	LoadedAsset->Modify();
	UPersistentData* PersistentData = Cast<UPersistentData>(LoadedAsset);
	if (!PersistentData)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to PersistentData failed"));
		return;
	}

	if (PersistentData->Milestones.Contains(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("PersistentData already serialized this"));
		return;
	}
	PersistentData->Milestones.Add(this);

	AsyncTask(ENamedThreads::Type::GameThread, [this, PersistentData]() {PersistentData->MarkPackageDirty();} );
}

void UMilestone::RefreshGUID()
{
	MilestoneID = FGuid::NewGuid();
	MarkPackageDirty();
}

#endif