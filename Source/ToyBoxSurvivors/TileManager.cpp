// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"

#include "Tile.h"

ATileManager::ATileManager()
{

}

void ATileManager::BeginPlay()
{
	Super::BeginPlay();

	CreateTileGrid();
}

void ATileManager::CreateTileGrid()
{
	FActorSpawnParameters spawnParameters = FActorSpawnParameters();
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector spawnLocation = FVector(0, 0, 0);
	FRotator spawnRotation = FRotator(0, 0, 0);
	auto zeroTile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnParameters);
	FVector2D numberOfTiles = FVector2D( GridSize.X / zeroTile->TileSize.X, GridSize.Y / zeroTile->TileSize.Y);
	FVector2D tileSize = FVector2D(zeroTile->TileSize.X, zeroTile->TileSize.Y);
	zeroTile->Destroy();
	zeroTile = nullptr;

	for (int i = 0; i < numberOfTiles.X; i++)
	{
		for (int j = 0; j < numberOfTiles.Y; j++)
		{
			spawnLocation.X = tileSize.X * i;
			spawnLocation.Y = tileSize.Y * j;
			auto tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
#if WITH_EDITOR
			tile->SetFolderPath("TileGrid");
#endif
			spawnLocation.X = -tileSize.X * i;
			spawnLocation.Y = tileSize.Y * j;
			tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
#if WITH_EDITOR
			tile->SetFolderPath("TileGrid");
#endif

			spawnLocation.X = tileSize.X * i;
			spawnLocation.Y = -tileSize.Y * j;
			tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
#if WITH_EDITOR
			tile->SetFolderPath("TileGrid");
#endif

			spawnLocation.X = -tileSize.X * i;
			spawnLocation.Y = -tileSize.Y * j;
			tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
#if WITH_EDITOR
			tile->SetFolderPath("TileGrid");
#endif
		}
	}
}
