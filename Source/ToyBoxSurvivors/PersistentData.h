// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PersistentData.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPersistentData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SurvivorGameInstance")
	TArray<class UUnlockableData*> Unlockables;

	UPROPERTY(EditDefaultsOnly, Category = "SurvivorGameInstance")
	TArray<class UMilestone*> Milestones;
};
