// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressionManager.h"

#include "SaveFile.h"
#include "SurvivorGameInstance.h"

void UProgressionManager::InjectInstance(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
}


void UProgressionManager::LoadSaveData(const USaveFile& SaveFile)
{
	CompletedMilestones = SaveFile.CompletedMilestones;
}


void UProgressionManager::MilestoneAchieved(EMilestones Milestone)
{
	CompletedMilestones |= Milestone;
	GameInstance->SaveGame();
}
