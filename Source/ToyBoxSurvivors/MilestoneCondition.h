// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Milestone.h"
#include "MilestoneCondition.generated.h"

USTRUCT(BlueprintType)
struct FStatRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStatsType RequiredStat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStatsDomain RequiredDomain;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RequiredAmount;
};

UCLASS(BlueprintType, EditInlineNew)
class TOONTANKS_API UMilestoneCondition : public UObject
{
	GENERATED_BODY()

public:
	UMilestoneCondition();
	
	bool ConditionsMet(class USurvivorGameInstance* GameInstance);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Unlock Conditions")
	TArray<FStatRequirement> StatRequirements;
	UPROPERTY(EditAnywhere, Category = "Unlock Conditions")
	TArray<UMilestone*> MilestoneRequirements;
};