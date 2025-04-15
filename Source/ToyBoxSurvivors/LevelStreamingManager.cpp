// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingManager.h"

#include "Tank.h"
#include "ToonTanksGameMode.h"

ULevelStreamingManager::ULevelStreamingManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(4.0f);
}

void ULevelStreamingManager::Initialize()
{
	World = GetWorld();
	GameMode = Cast<AToonTanksGameMode>(GetOwner());
	Player = GameMode->GetPlayer();

	for (auto* Level : World->GetStreamingLevels())
	{
		Level->bDisableDistanceStreaming = true;
	}
}

void ULevelStreamingManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLevelStreaming();
}

void ULevelStreamingManager::UpdateLevelStreaming() const
{
	for (auto* Level : World->GetStreamingLevels())
	{
		FVector PlayerPosition = Player->GetActorLocation();
		FBox LevelBounds = Level->GetStreamingVolumeBounds();
		FVector LevelPosition = LevelBounds.GetClosestPointTo(PlayerPosition);

		float DistanceToPlayer = (LevelPosition - PlayerPosition).SquaredLength();

		bool InRangeToLoad = DistanceToPlayer < LoadDistance * LoadDistance;
		bool IsLoaded = Level->IsLevelLoaded();

		if (InRangeToLoad && !IsLoaded)
		{
			Level->SetShouldBeLoaded(true);
			Level->SetShouldBeVisible(true);
			Level->bShouldBlockOnLoad = true;
		}
		else if (!InRangeToLoad && IsLoaded)
		{
			Level->SetShouldBeLoaded(false);
			Level->SetShouldBeVisible(false);
		}
	}
}
