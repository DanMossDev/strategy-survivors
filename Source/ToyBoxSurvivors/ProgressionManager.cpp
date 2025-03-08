// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressionManager.h"

#include "EventDispatcher.h"
#include "SaveFile.h"
#include "SurvivorGameInstance.h"

UProgressionManager::UProgressionManager()
{
	CompletedMilestones = EMilestoneType::None;
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


void UProgressionManager::MilestoneAchieved(const EMilestoneType Milestone)
{
	CompletedMilestones |= Milestone;

	UEventDispatcher::IncomingMilestoneUnlocked(Milestone);
	GameInstance->SaveGame();
}

bool UProgressionManager::AreMilestonesAchieved(const EMilestoneType Milestones) const
{
	return EnumHasAllFlags(CompletedMilestones, Milestones);
}
