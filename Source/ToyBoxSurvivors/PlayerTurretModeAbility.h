// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAbility.h"
#include "PlayerTurretModeAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPlayerTurretModeAbility : public UPlayerAbility
{
	GENERATED_BODY()

protected:
	virtual void CastAbility() override;
	virtual void ProcessAbility(float DeltaTime, float RatioCompleted) override;
	virtual void FinishCastingAbility() override;

	UPROPERTY(EditAnywhere, Category = "RotatingTurret")
	FVector TurretTargetScale;
	UPROPERTY(EditAnywhere, Category = "RotatingTurret")
	FVector TurretTargetPosition;

	FVector TurretStartScale;
	FVector TurretStartPosition;
};
