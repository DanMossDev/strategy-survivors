// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

UCLASS()
class TOONTANKS_API ATileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileManager();

protected:
	virtual void BeginPlay() override;

	void CreateTileGrid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Grid")
	FVector2D GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Grid")
	TSubclassOf<class ATile> TemplateTile;
};
