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

	if (NonStatMilestoneUnlock !=ENonStatMilestones::None && GameInstance->GetProgressionManager()->AreMilestonesAchieved(NonStatMilestoneUnlock))
		return;

	UEventDispatcher::OnStatChanged.AddDynamic(this, &UMilestone::OnStatChanged);
	UEventDispatcher::OnMilestoneUnlocked.AddDynamic(this, &UMilestone::OnMilestoneUnlocked);
}

void UMilestone::Cleanup()
{
	if (UEventDispatcher::OnStatChanged.IsAlreadyBound(this, &UMilestone::OnStatChanged))
		UEventDispatcher::OnStatChanged.RemoveDynamic(this, &UMilestone::OnStatChanged);
	if (UEventDispatcher::OnMilestoneUnlocked.IsAlreadyBound(this, &UMilestone::OnMilestoneUnlocked))
		UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &UMilestone::OnMilestoneUnlocked);
}

void UMilestone::OnStatChanged(EStatsType ChangedStat)
{
	if (!MilestoneCondition->ConditionsMet(GameInstance))
		return;

	UEventDispatcher::OnStatChanged.RemoveDynamic(this, &UMilestone::OnStatChanged);
	UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &UMilestone::OnMilestoneUnlocked);
	GameInstance->GetProgressionManager()->MilestoneAchieved(NonStatMilestoneUnlock);
}

void UMilestone::OnMilestoneUnlocked(ENonStatMilestones UnlockedMilestone)
{
	if (!MilestoneCondition->ConditionsMet(GameInstance))
		return;

	Cleanup();
	GameInstance->GetProgressionManager()->MilestoneAchieved(NonStatMilestoneUnlock);
}

#if WITH_EDITOR
void UMilestone::PostLoad()
{
	Super::PostLoad();
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
#endif