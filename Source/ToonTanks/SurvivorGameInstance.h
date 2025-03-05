// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveFile.h"
#include "Engine/GameInstance.h"
#include "SurvivorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API USurvivorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	void LoadGame();

	USaveFile* GetSaveFile() const {return CurrentSaveGame;}

protected:
	UPROPERTY()
	USaveFile* CurrentSaveGame;
	FString SaveSlot = TEXT("save");

	UPROPERTY(EditDefaultsOnly, Category = "SurvivorGameInstance")
	TArray<class UUnlockableData*> Unlockables;
};
