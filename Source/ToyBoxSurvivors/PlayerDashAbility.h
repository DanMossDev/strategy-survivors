// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAbility.h"
#include "PlayerDashAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UPlayerDashAbility : public UPlayerAbility
{
	GENERATED_BODY()

public:
	virtual void InjectOwner(class UPlayerAbilitySystem* Owner) override;
protected:
	virtual void CastAbility() override;
	virtual void ProcessAbility(float DeltaTime, float RatioCompleted) override;
	virtual void FinishCastingAbility() override;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	float DashSpeed;
	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	FLinearColor DashColor;

	UPROPERTY()
	class ATank* Player;
	UPROPERTY()
	class UPlayerHealthComponent* PlayerHealthComponent;
};
