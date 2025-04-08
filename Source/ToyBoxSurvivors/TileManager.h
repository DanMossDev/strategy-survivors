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
	void UpdateTileGrid();

	FVector2D WorldSpaceToTileSpace(const FVector& Worldspace) const;
	FVector TileSpaceToWorldSpace(const FVector2D& TileSpace) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Grid")
	FVector2D GridSize;

	UPROPERTY()
	FVector2D GridSizeTileSpace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Grid")
	int32 TileSpaceDistanceToMoveAt = 5;

	UPROPERTY()
	FVector2D TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tile Grid")
	TSubclassOf<class ATile> TemplateTile;

	UPROPERTY()
	TArray<ATile*> Tiles;

	UPROPERTY()
	class AToonTanksGameMode* GameMode;
	UPROPERTY()
	class ATank* Player;

public:
	virtual void Tick(float DeltaTime) override;
};
