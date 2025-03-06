// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnlockableData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UUnlockableData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unlockable")
	bool BeginsUnlocked = false;
	
	bool IsUnlocked = true;

	void Unlock(UObject* worldContextObject);
};
