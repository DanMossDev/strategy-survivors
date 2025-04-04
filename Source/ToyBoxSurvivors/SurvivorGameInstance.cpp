// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorGameInstance.h"

#include "MilestoneCondition.h"
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
		CurrentSaveGame->PersistentStats.Stats = StatsManager->GetPersistentStats()->GetStats();
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

TArray<UMilestone*> USurvivorGameInstance::GetMilestonesForCharacter(UPlayableCharacter* Character)
{
	TArray<UMilestone*> CharacterMilestones;

	for (auto milestone : PersistentData->Milestones)
	{
		if (milestone->MilestoneCondition == nullptr && Character == nullptr)
			CharacterMilestones.Add(milestone);
		else if (milestone->MilestoneCondition != nullptr && milestone->MilestoneCondition->RequiredCharacter == Character)
			CharacterMilestones.Add(milestone);
	}

	return CharacterMilestones;
}


TMap<EStatsType, float> USurvivorGameInstance::GetCharacterStats(UPlayableCharacter* Character)
{
	if (CurrentSaveGame->CharacterSpecificStats.Contains(Character->Name))
		return CurrentSaveGame->CharacterSpecificStats[Character->Name].Stats;

	return TMap<EStatsType, float>();
}

void USurvivorGameInstance::SaveCharacterStats(UStatsData* CharacterStats)
{
	if (!CurrentSaveGame->CharacterSpecificStats.Contains(CurrentPlayerCharacter->Name))
		CurrentSaveGame->CharacterSpecificStats.Add(CurrentPlayerCharacter->Name, FSavedStats());
	
	CurrentSaveGame->CharacterSpecificStats[CurrentPlayerCharacter->Name].Stats = CharacterStats->GetStats();
}
