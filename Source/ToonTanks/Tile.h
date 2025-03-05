// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TOONTANKS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tiles")
	FVector2D TileSize = FVector2D(100,100);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tiles")
	FVector2D TileCoordinates = FVector2D(0,0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tiles")
	class UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tiles")
	class ATileMesh* TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tiles")
	EElementalType CurrentElement = EElementalType::None;

	void SetElement(EElementalType Element);
	void HitByElement(EElementalType Element);

	UFUNCTION(BlueprintImplementableEvent)
	void OnElementChanged(EElementalType Element);

protected:
	virtual void BeginPlay() override;

	float RemainingLifetime;

	bool GetOverlappingEnemies(TArray<FOverlapResult>& OverlappingActors);
	void CheckOverlappingProjectiles();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tiles")
	TSubclassOf<ATileMesh> TileClass;
	class AToonTanksGameMode* GameMode;

public:	
	virtual void Tick(float DeltaTime) override;

};
