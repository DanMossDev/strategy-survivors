// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnlockableData.generated.h"

enum class EMilestoneType : uint8;
/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UUnlockableData : public UDataAsset
{
	GENERATED_BODY()

public:
	void Init(class USurvivorGameInstance* Instance);
	void Cleanup();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString Name;
	
	bool IsUnlocked = false;

	UFUNCTION()
	void OnMilestoneCompleted(EMilestoneType Milestone);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	EMilestoneType UnlockCondition;

	USurvivorGameInstance* GameInstance;

#if WITH_EDITOR
	virtual void PostLoad() override;
#endif
};
