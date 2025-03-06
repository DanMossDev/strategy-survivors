// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressionManager.h"
#include "GameFramework/SaveGame.h"
#include "SaveFile.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API USaveFile : public USaveGame
{
	GENERATED_BODY()

public:
	USaveFile();

	UPROPERTY()
	TMap<FString, bool> UnlockedItems;

	UPROPERTY(BlueprintReadWrite)
	float Volume;
	UPROPERTY(BlueprintReadWrite)
	float SFXVolume;
	UPROPERTY(BlueprintReadWrite)
	float MusicVolume;

	UPROPERTY(BlueprintReadWrite)
	EMilestones CompletedMilestones;
};
