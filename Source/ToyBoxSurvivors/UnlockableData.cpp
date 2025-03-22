// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockableData.h"

#include "MilestoneCondition.h"
#include "PersistentData.h"

void UUnlockableData::Init(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
}

bool UUnlockableData::IsUnlocked() const
{
	return !UnlockCondition || UnlockCondition->GetIsUnlocked();
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
	LoadedAsset->Modify();
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
	
	AsyncTask(ENamedThreads::Type::GameThread, [this, PersistentData]() {PersistentData->MarkPackageDirty();} );
}
#endif