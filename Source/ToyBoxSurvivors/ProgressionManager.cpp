// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressionManager.h"

#include "SaveFile.h"
#include "SurvivorGameInstance.h"

FMilestoneUnlocked UProgressionManager::OnMilestoneUnlocked;

void UProgressionManager::InjectInstance(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
}


void UProgressionManager::LoadSaveData(const USaveFile& SaveFile)
{
	CompletedMilestones = SaveFile.CompletedMilestones;
}


void UProgressionManager::MilestoneAchieved(const EMilestones Milestone)
{
	CompletedMilestones |= Milestone;

	OnMilestoneUnlocked.Broadcast(Milestone);
	GameInstance->SaveGame();
}
