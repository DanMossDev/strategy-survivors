// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "TileMesh.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATileMesh : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ATileMesh();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupMaterials(UMaterialInterface* Material);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TileMesh")
	class UPoolableComponent* Poolable;

	void RefreshVisuals();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TileMesh")
	FVector LocationOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TileMesh")
	float MaxRotationOffset = 40.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TileMesh")
	float MinRotationOffset = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TileMesh")
	float TimeToComplete = 0.25f;

	FVector TargetLocation;
	FVector OffsetLocation;
	FRotator TargetRotation;
	FRotator OffsetRotation;
	float t;
	
	UFUNCTION()
	void OnGetFromPool();

public:
	virtual void Tick(float DeltaTime) override;
};
