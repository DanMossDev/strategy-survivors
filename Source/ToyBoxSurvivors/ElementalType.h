// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EElementalType : uint8
{
	None UMETA(DisplayName = "None"),
	Fire UMETA(DisplayName = "Fire"),
	Oil UMETA(DisplayName = "Oil"),
	Water UMETA(DisplayName = "Water"),
	Ice UMETA(DisplayName = "Ice"),
	Lightning UMETA(DisplayName = "Lightning"),
};
