// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressionManager.h"

#include "EventDispatcher.h"
#include "SaveFile.h"
#include "SurvivorGameInstance.h"

UProgressionManager::UProgressionManager()
{
	CompletedMilestones = TArray<FGuid>();
}


void UProgressionManager::InjectInstance(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;

	LoadSaveData(GameInstance->GetSaveFile());
}


void UProgressionManager::LoadSaveData(USaveFile* SaveFile)
{
	CompletedMilestones = SaveFile->CompletedMilestones;
}


void UProgressionManager::MilestoneAchieved(UMilestone* Milestone)
{
	CompletedMilestones.Add(Milestone->MilestoneID);

	UEventDispatcher::IncomingMilestoneUnlocked(Milestone);
}
