// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockableData.h"

#include "EventDispatcher.h"
#include "PersistentData.h"
#include "SurvivorGameInstance.h"

void UUnlockableData::Init(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
	IsUnlocked = false;
	
	if (EnumHasAllFlags(GameInstance->GetSaveFile()->CompletedMilestones, UnlockCondition))
		IsUnlocked = true;
	else
		UEventDispatcher::OnMilestoneUnlocked.AddDynamic(this, &UUnlockableData::OnMilestoneCompleted);
}

void UUnlockableData::Cleanup()
{
	if (UEventDispatcher::OnMilestoneUnlocked.IsAlreadyBound(this, &UUnlockableData::OnMilestoneCompleted))
		UEventDispatcher::OnMilestoneUnlocked.RemoveDynamic(this, &UUnlockableData::OnMilestoneCompleted);
}

void UUnlockableData::OnMilestoneCompleted(EMilestoneType Milestone)
{
	if (EnumHasAllFlags(Milestone, UnlockCondition))
		IsUnlocked = true;

	Cleanup();
}

#if WITH_EDITOR
void UUnlockableData::PostLoad()
{
	Super::PostLoad();
	UE_LOG(LogTemp, Display, TEXT("Loading unlockable"));

	auto LoadedAsset = StaticLoadObject(UPersistentData::StaticClass(), nullptr, TEXT("/Game/Data/DA_PersistentData.DA_PersistentData"));
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load asset for PersistentData"));
		return;
	}
	UPersistentData* PersistentData = Cast<UPersistentData>(LoadedAsset);
	if (!PersistentData)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to PersistentData failed"));
		return;
	}

	if (PersistentData->Unlockables.Contains(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("PersistentData already serialized this"));
		return;
	}
	PersistentData->Unlockables.Add(this);
	UE_LOG(LogTemp, Display, TEXT("Successfully loaded unlockable"));
}
#endif