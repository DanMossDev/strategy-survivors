// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnlockableData.generated.h"

enum class ENonStatMilestones : uint8;
/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API UUnlockableData : public UDataAsset
{
	GENERATED_BODY()

public:
	void Init(class USurvivorGameInstance* Instance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	UTexture2D* Sprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	int32 MaxLevel = 4;
	
	bool IsUnlocked() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unlockable")
	class UMilestone* UnlockCondition;


	USurvivorGameInstance* GameInstance;

#if WITH_EDITOR
	virtual void PostLoad() override;
#endif
};
