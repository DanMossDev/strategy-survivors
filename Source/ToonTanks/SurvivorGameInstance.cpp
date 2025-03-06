// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorGameInstance.h"

#include "WeaponInfo.h"
#include "Kismet/GameplayStatics.h"

void USurvivorGameInstance::Init()
{
	Super::Init();

	LoadGame();
}

void USurvivorGameInstance::SaveGame()
{
	if (!CurrentSaveGame)
		CurrentSaveGame = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));

	if (CurrentSaveGame)
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlot, 0);
	else
		UE_LOG(LogTemp, Error, TEXT("Save file could not be created"));
}

void USurvivorGameInstance::LoadGame()
{
	CurrentSaveGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));

	if (!CurrentSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Save file could not be loaded, creating new data"));
		SaveGame();
		return;
	}

	for (auto unlockable : Unlockables)
	{
		unlockable->IsUnlocked = unlockable->BeginsUnlocked;

		if (UUnlockableData* unlockableData = Cast<UUnlockableData>(unlockable))
			if (CurrentSaveGame->UnlockedItems.Contains(unlockableData->Name))
				unlockableData->IsUnlocked = CurrentSaveGame->UnlockedItems[unlockableData->Name];
	}
}
