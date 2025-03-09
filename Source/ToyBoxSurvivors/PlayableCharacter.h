// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnlockableData.h"
#include "PlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPlayableCharacter : public UUnlockableData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable Character")
	TSubclassOf<class ATank> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable Character")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable Character")
	UTexture2D* Sprite;
};
