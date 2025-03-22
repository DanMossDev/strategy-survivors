// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorGameInstance.h"

#include "PersistentData.h"
#include "PlayableCharacter.h"
#include "ProgressionManager.h"
#include "WeaponInfo.h"
#include "Kismet/GameplayStatics.h"

void USurvivorGameInstance::Init()
{
	Super::Init();

	LoadGame();
	ProgressionManager->InjectInstance(this);
	StatsManager->InjectInstance(this);
	for (auto milestone : PersistentData->Milestones)
		milestone->InjectInstance(this);
}

void USurvivorGameInstance::Shutdown()
{
	Super::Shutdown();

	StatsManager->Cleanup();
	for (auto milestone : PersistentData->Milestones)
		milestone->Cleanup();
}

void USurvivorGameInstance::SaveGame()
{
	if (!CurrentSaveGame)
		CurrentSaveGame = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));

	if (CurrentSaveGame)
	{
		CurrentSaveGame->CompletedMilestones = ProgressionManager->GetCompletedMilestones();
		CurrentSaveGame->PersistentStats = StatsManager->GetPersistentStats()->GetStats();
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlot, 0);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Save file could not be created"));
}

void USurvivorGameInstance::LoadGame()
{
	CurrentSaveGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));

	if (!CurrentSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Save file could not be loaded, creating new data"));
		CurrentSaveGame = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
		UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlot, 0);
	}

	for (auto unlockable : PersistentData->Unlockables)
	{
		unlockable->Init(this);
	}
}

TArray<UPlayableCharacter*> USurvivorGameInstance::GetAllUnlockedPlayerCharacters()
{
	TArray<UPlayableCharacter*> UnlockedCharacters;
	for (auto unlockable : PersistentData->Unlockables)
	{
		if (auto character = Cast<UPlayableCharacter>(unlockable))
		{
			if (character->IsUnlocked())
				UnlockedCharacters.Add(character);
		}
	}
	return UnlockedCharacters;
}
