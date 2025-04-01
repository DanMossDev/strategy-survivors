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
	virtual void Shutdown() override;
	
	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	TArray<class UPlayableCharacter*> GetAllUnlockedPlayerCharacters();
	
	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	USaveFile* GetSaveFile() const {return CurrentSaveGame;}

	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	TMap<EStatsType, float> GetCharacterStats(UPlayableCharacter* Character);

	void SaveCharacterStats(UStatsData* CharacterStats);

	class UProgressionManager* GetProgressionManager() const {return ProgressionManager;}

	UFUNCTION(BlueprintCallable, Category = "SurvivorGameInstance")
	class UStatsManager* GetStatsManager() const {return StatsManager;}

	UPROPERTY(BlueprintReadWrite, Category = "SurvivorGameInstance")
	TSubclassOf<class ATank> PlayerPawn;

	UPROPERTY(BlueprintReadWrite, Category = "SurvivorGameInstance")
	class UPlayableCharacter* CurrentPlayerCharacter;

protected:
	UPROPERTY()
	USaveFile* CurrentSaveGame;
	FString SaveSlot = TEXT("save");
	
	UPROPERTY(EditDefaultsOnly, Category = "SurvivorGameInstance")
	class UPersistentData* PersistentData;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "SurvivorGameInstance")
	UProgressionManager* ProgressionManager;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "SurvivorGameInstance")
	UStatsManager* StatsManager;

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY(EditDefaultsOnly, Category = "SurvivorGameInstance")
	bool ShowDebugUnlocks = true;
#endif
};
