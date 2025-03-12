// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameEndData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UGameEndData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="GameEndData")
	bool IsWin;
	UPROPERTY(BlueprintReadOnly, Category="GameEndData")
	int32 EnemiesKilled;
	UPROPERTY(BlueprintReadOnly, Category="GameEndData")
	float TimeSurvived;
	UPROPERTY(BlueprintReadOnly, Category="GameEndData")
	float DistanceTravelled;
};
