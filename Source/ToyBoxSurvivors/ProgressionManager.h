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

	void MilestoneAchieved(const ENonStatMilestones Milestone);

	bool AreMilestonesAchieved(const ENonStatMilestones Milestones) const;

	ENonStatMilestones GetCompletedMilestones() const { return CompletedMilestones; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "ProgressionManager")
	ENonStatMilestones CompletedMilestones;

	USurvivorGameInstance* GameInstance;
};