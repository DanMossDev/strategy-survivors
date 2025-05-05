// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSelectionDisplayer.generated.h"

UCLASS()
class TOONTANKS_API ACharacterSelectionDisplayer : public AActor
{
	GENERATED_BODY()
	
public:
	ACharacterSelectionDisplayer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Select")
	class UPlayableCharacter* CurrentlySelectedCharacter;

	UPROPERTY()
	TMap<UPlayableCharacter*, UStaticMeshComponent*> CharacterMeshLookup;

	UFUNCTION(BlueprintCallable, Category = "Character Select")
	void ShowNewCharacter(UPlayableCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	void AddToCharacterMeshLookup(UPlayableCharacter* Character, UStaticMeshComponent* MeshComponent);

protected:
	void SetAllChildrenVisible(UStaticMeshComponent* MeshComponent, bool Visible);
};
