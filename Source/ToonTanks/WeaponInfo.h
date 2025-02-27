// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Engine/DataAsset.h"
#include "WeaponInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	TSubclassOf<UWeapon> WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	bool IsUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	UTexture2D* Sprite;

};
