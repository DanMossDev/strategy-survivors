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
};
