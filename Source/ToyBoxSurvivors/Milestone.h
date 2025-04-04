// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatsData.h"
#include "Milestone.generated.h"

// UENUM(BlueprintType, meta=(Bitflags))
// enum class ENonStatMilestones : uint8
// {
// 	None UMETA(DisplayName = "None"),
// };
//
// ENUM_CLASS_FLAGS(ENonStatMilestones);

UCLASS(BlueprintType)
class TOONTANKS_API UMilestone : public UDataAsset
{
	GENERATED_BODY()

public:
	void InjectInstance(class USurvivorGameInstance* Instance);
	void Cleanup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	FString MilestoneName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	FText MilestoneDisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	FText MilestoneFlavourText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Milestones")
	UTexture2D* MilestoneIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Milestones")
	class UMilestoneCondition* MilestoneCondition;

	UPROPERTY(VisibleAnywhere, Category = "Milestones")
	FGuid MilestoneID;

	void OnMilestoneUnlocked();
	UFUNCTION(BlueprintCallable, Category = "Milestones")
	bool GetIsUnlocked() const;

	bool IsUnlocked = false;

protected:
	USurvivorGameInstance* GameInstance;

	UFUNCTION()
	void OnStatChanged(EStatsType ChangedStat);

#if WITH_EDITOR
	virtual void PostLoad() override;

public:
	UFUNCTION(CallInEditor, Category = "Milestones")
	void RefreshGUID();
#endif
};
