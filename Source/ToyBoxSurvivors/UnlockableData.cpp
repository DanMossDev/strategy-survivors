// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockableData.h"

#include "MilestoneCondition.h"
#include "PersistentData.h"
#include "SurvivorGameInstance.h"

void UUnlockableData::Init(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
}

bool UUnlockableData::IsUnlocked() const
{
#if WITH_EDITOR
	if (IsDebug)
		return GameInstance->ShowDebugUnlocks;
#endif
	return !UnlockCondition || UnlockCondition->GetIsUnlocked();
}

#if WITH_EDITOR
void UUnlockableData::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostLoad();

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

	if (PersistentData->Unlockables.Contains(this))
	{
		return;
	}
	PersistentData->Unlockables.Add(this);
	
	PersistentData->MarkPackageDirty();
}
#endif