// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProgressionManager.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta=(Bitflags))
enum class EMilestones : uint8
{
	None UMETA(DisplayName = "None"),
	FireGunUnlocked UMETA(DisplayName = "FireGunUnlocked"),
	WaterGunUnlocked UMETA(DisplayName = "WaterGunUnlocked"),
	IceGunUnlocked UMETA(DisplayName = "IceGunUnlocked"),
};

ENUM_CLASS_FLAGS(EMilestones);

UCLASS(Blueprintable, EditInLineNew)
class TOONTANKS_API UProgressionManager : public UObject
{
	GENERATED_BODY()

public:
	void InjectInstance(class USurvivorGameInstance* Instance);
	
	void LoadSaveData(const class USaveFile& SaveFile);

	void MilestoneAchieved(EMilestones Milestone);

protected:
	UPROPERTY(VisibleAnywhere, Category = "ProgressionManager")
	EMilestones CompletedMilestones;
	
	UPROPERTY(EditAnywhere, Category = "ProgressionManager")
	TMap<EMilestones, class UWeaponInfo*> MilestoneWeaponUnlocks;
	UPROPERTY(EditAnywhere, Category = "ProgressionManager")
	TMap<EMilestones, class UStatBoost*> MilestoneStatUnlocks;

	USurvivorGameInstance* GameInstance;
};