// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAbility.h"
#include "PlayerBouncingBulletsAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPlayerBouncingBulletsAbility : public UPlayerAbility
{
	GENERATED_BODY()

protected:
	virtual void CastAbility() override;
	//virtual void ProcessAbility(float DeltaTime, float RatioCompleted) override;
	virtual void FinishCastingAbility() override;
};
