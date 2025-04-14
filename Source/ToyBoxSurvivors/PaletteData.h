// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PaletteData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UPaletteData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor BaseColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor SecondaryColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor TertiaryColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor TextColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor HighlightedTextColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor InteractableColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PaletteData")
	FLinearColor HighlightedInteractableColour;
};
