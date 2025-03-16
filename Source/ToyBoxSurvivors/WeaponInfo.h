// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnlockableData.h"
#include "Weapon.h"
#include "Engine/DataAsset.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UWeaponInfo : public UUnlockableData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	TSubclassOf<UWeapon> WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	bool HasEvolution = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	TArray<UUnlockableData*> EvolutionRequirements;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString EvolvedName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString EvolvedDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	UTexture2D* EvolvedSprite;
};
