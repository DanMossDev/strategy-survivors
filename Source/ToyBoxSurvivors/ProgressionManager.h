// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Milestone.h"
#include "ProgressionManager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UProgressionManager : public UObject
{
	GENERATED_BODY()

public:
	UProgressionManager();
	void InjectInstance(class USurvivorGameInstance* Instance);
	
	void LoadSaveData(class USaveFile* SaveFile);

	void MilestoneAchieved(const EMilestoneType Milestone);

	bool AreMilestonesAchieved(const EMilestoneType Milestones) const;

	EMilestoneType GetCompletedMilestones() const { return CompletedMilestones; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "ProgressionManager")
	EMilestoneType CompletedMilestones;

	USurvivorGameInstance* GameInstance;
};