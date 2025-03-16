// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityStats.h"
#include "UnlockableData.h"
#include "Engine/DataAsset.h"
#include "StatBoost.generated.h"

UCLASS(BlueprintType)
class TOONTANKS_API UStatBoost : public UUnlockableData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "StatBoost")
	UEntityStats* Effect;
};
