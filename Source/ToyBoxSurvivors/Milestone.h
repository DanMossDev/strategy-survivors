// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsData.h"
#include "Milestone.generated.h"

UENUM(BlueprintType, meta=(Bitflags))
enum class EMilestoneType : uint8
{
	None UMETA(DisplayName = "None"),
	FireGunUnlocked UMETA(DisplayName = "FireGunUnlocked"),
	WaterGunUnlocked UMETA(DisplayName = "WaterGunUnlocked"),
	IceGunUnlocked UMETA(DisplayName = "IceGunUnlocked"),
	TenEnemiesKilled UMETA(DisplayName = "TenEnemiesKilled"),
};

ENUM_CLASS_FLAGS(EMilestoneType);
UCLASS(BlueprintType)
class TOONTANKS_API UMilestone : public UDataAsset
{
	GENERATED_BODY()

public:
	void InjectInstance(class USurvivorGameInstance* Instance);
	void Cleanup();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	EMilestoneType Milestone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Milestones")
	class UMilestoneCondition* MilestoneCondition;

protected:
	USurvivorGameInstance* GameInstance;

	UFUNCTION()
	void OnStatChanged(EStatsType ChangedStat);
	UFUNCTION()
	void OnMilestoneUnlocked(EMilestoneType UnlockedMilestone);

#if WITH_EDITOR
	virtual void PostLoad() override;
#endif
};
