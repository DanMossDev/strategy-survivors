// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravitateToPlayerInRange.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UGravitateToPlayerInRange : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravitateToPlayerInRange();

	void Init();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float DeltaTime);
	bool CheckPlayerInRange();

	class ATank* PlayerTank;
	bool IsBeingAttracted = false;
	bool WasBeingAttractedLastFrame = false;

	float LerpRatio = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
