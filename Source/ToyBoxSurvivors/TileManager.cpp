// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"

#include "ObjectPoolComponent.h"
#include "Tank.h"
#include "Tile.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

ATileManager::ATileManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATileManager::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Player = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	CreateTileGrid();
}

void ATileManager::CreateTileGrid()
{
	FActorSpawnParameters spawnParameters = FActorSpawnParameters();
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector spawnLocation = FVector(0, 0, 0);
	FRotator spawnRotation = FRotator(0, 0, 0);
	auto zeroTile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnParameters);
	GridSizeTileSpace = FVector2D( GridSize.X / zeroTile->TileSize.X, GridSize.Y / zeroTile->TileSize.Y);
	TileSize = FVector2D(zeroTile->TileSize.X, zeroTile->TileSize.Y);
	zeroTile->Destroy();
	zeroTile = nullptr;

	TArray<FVector2D> TilePositions;
	ATile* tile;
	
	for (int i = 0; i < GridSizeTileSpace.X; i++)
	{
		for (int j = 0; j < GridSizeTileSpace.Y; j++)
		{
			if (!TilePositions.Contains(FVector2D(i, j)))
			{
				spawnLocation.X = TileSize.X * i;
				spawnLocation.Y = TileSize.Y * j;
				tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
				tile->TileCoordinates = FVector2D(i, j);

				Tiles.Add(tile);
				tile = nullptr;
				
				TilePositions.Add(FVector2D(i, j));
			}
			

			if (!TilePositions.Contains(FVector2D(-i, j)))
			{
				spawnLocation.X = TileSize.X * -i;
				spawnLocation.Y = TileSize.Y * j;
				tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
				tile->TileCoordinates = FVector2D(-i, j);

				Tiles.Add(tile);
				tile = nullptr;
				
				TilePositions.Add(FVector2D(-i, j));
			}

			if (!TilePositions.Contains(FVector2D(i, -j)))
			{
				spawnLocation.X = TileSize.X * i;
				spawnLocation.Y = TileSize.Y * -j;
				tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
				tile->TileCoordinates = FVector2D(i, -j);

				Tiles.Add(tile);
				tile = nullptr;
				
				TilePositions.Add(FVector2D(i, -j));
			}

			if (!TilePositions.Contains(FVector2D(-i, -j)))
			{
				spawnLocation.X = TileSize.X * -i;
				spawnLocation.Y = TileSize.Y * -j;
				tile = GetWorld()->SpawnActor<ATile>(TemplateTile, spawnLocation, spawnRotation, spawnParameters);
				tile->TileCoordinates = FVector2D(-i, -j);

				Tiles.Add(tile);
				tile = nullptr;
				
				TilePositions.Add(FVector2D(-i, -j));
			}
		}
	}
}

void ATileManager::UpdateTileGrid()
{
	FVector2D playerCoordinates = WorldSpaceToTileSpace(Player->GetActorLocation());

	int32 moved = 0;

	for (auto tile : Tiles)
	{
		FVector2D diff = playerCoordinates - tile->TileCoordinates;
		FVector2D startCoords = tile->TileCoordinates;

		if (diff.SquaredLength() < TileSpaceDistanceToMoveAt * TileSpaceDistanceToMoveAt)
			continue;

		if (diff.X > TileSpaceDistanceToMoveAt)
		{
			tile->TileCoordinates.X += GridSizeTileSpace.X * 2 - 1;
		}
		else if (diff.X <= -TileSpaceDistanceToMoveAt)
		{
			tile->TileCoordinates.X -= GridSizeTileSpace.X * 2 - 1;
		}

		if (diff.Y > TileSpaceDistanceToMoveAt)
		{
			tile->TileCoordinates.Y += GridSizeTileSpace.Y * 2 - 1;
		}
		else if (diff.Y <= -TileSpaceDistanceToMoveAt)
		{
			tile->TileCoordinates.Y -= GridSizeTileSpace.Y * 2 - 1;
		}

		if (tile->TileCoordinates != startCoords)
		{
			tile->SetActorLocation(TileSpaceToWorldSpace(tile->TileCoordinates));
			moved++;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("moved %d tiles"), moved);
}


void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTileGrid();
}

FVector2D ATileManager::WorldSpaceToTileSpace(const FVector& Worldspace) const
{
	return FVector2D(FMath::RoundToInt32(Worldspace.X / TileSize.X), FMath::RoundToInt32(Worldspace.Y / TileSize.Y));
}

FVector ATileManager::TileSpaceToWorldSpace(const FVector2D& TileSpace) const
{
	return FVector(TileSpace.X * TileSize.X, TileSpace.Y * TileSize.Y, 0);
}

