// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsData.h"
#include "Milestone.generated.h"

UENUM(BlueprintType, meta=(Bitflags))
enum class ENonStatMilestones : uint8
{
	None UMETA(DisplayName = "None"),
	FireGunUnlocked UMETA(DisplayName = "Fire Gun Unlocked"),
	WaterGunUnlocked UMETA(DisplayName = "Water Gun Unlocked"),
	IceGunUnlocked UMETA(DisplayName = "Ice Gun Unlocked"),
	OilGunUnlocked UMETA(DisplayName = "Oil Gun Unlocked")
};

ENUM_CLASS_FLAGS(ENonStatMilestones);

UCLASS(BlueprintType)
class TOONTANKS_API UMilestone : public UDataAsset
{
	GENERATED_BODY()

public:
	void InjectInstance(class USurvivorGameInstance* Instance);
	void Cleanup();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	ENonStatMilestones NonStatMilestoneUnlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Milestones")
	class UMilestoneCondition* MilestoneCondition;

protected:
	USurvivorGameInstance* GameInstance;

	UFUNCTION()
	void OnStatChanged(EStatsType ChangedStat);
	UFUNCTION()
	void OnMilestoneUnlocked(ENonStatMilestones UnlockedMilestone);

#if WITH_EDITOR
	virtual void PostLoad() override;
#endif
};
