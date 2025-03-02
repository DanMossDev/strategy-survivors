// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityStats.h"
#include "Engine/DataAsset.h"
#include "StatBoost.generated.h"

UCLASS(BlueprintType)
class TOONTANKS_API UStatBoost : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatBoost")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatBoost")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatBoost")
	UTexture2D* Sprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatBoost")
	bool IsUnlocked = true;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "StatBoost")
	UEntityStats* Effect;
};
