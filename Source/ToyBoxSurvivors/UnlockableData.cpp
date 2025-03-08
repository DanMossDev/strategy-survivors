// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockableData.h"

#include "EventDispatcher.h"
#include "SurvivorGameInstance.h"

void UUnlockableData::Init(USurvivorGameInstance* Instance)
{
	GameInstance = Instance;
	IsUnlocked = false;
	
	if ( EnumHasAllFlags(GameInstance->GetSaveFile()->CompletedMilestones, UnlockCondition) )
		IsUnlocked = true;
	// else
	// 	EventDispatcher::OnMilestoneUnlocked.AddDynamic(this, &UUnlockableData::OnMilestoneCompleted);
}

void UUnlockableData::OnMilestoneCompleted(EMilestoneType Milestone)
{
	if ( EnumHasAllFlags( Milestone, UnlockCondition ) )
		IsUnlocked = true;
}
