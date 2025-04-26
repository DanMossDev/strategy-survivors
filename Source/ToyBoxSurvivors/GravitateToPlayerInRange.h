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
	UGravitateToPlayerInRange();

	void Init();

protected:
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float DeltaTime);
	bool CheckPlayerInRange();

	UPROPERTY()
	class ATank* PlayerTank;
	bool IsBeingAttracted = false;
	bool WasBeingAttractedLastFrame = false;

	float LerpRatio = 0.0f;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
