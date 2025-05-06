// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnlockableData.h"
#include "Components/ActorComponent.h"
#include "PlayerAbility.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UPlayerAbility : public UUnlockableData
{
	GENERATED_BODY()

public:
	virtual void InjectOwner(class UPlayerAbilitySystem* Owner);
	virtual bool TryCastAbility();
	virtual void UpdateAbility(float DeltaTime);

	bool IsMovementLocked() const;
	bool IsShootingLocked() const;
	bool IsCastingLocked() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	bool CanBeEquipped = true;

protected:
	virtual void CastAbility();
	virtual void ProcessAbility(float DeltaTime, float RatioCompleted) {}
	virtual void FinishCastingAbility();

	// UFUNCTION(BlueprintImplementableEvent)
	// void OnAbilityStarted();
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnAbilityUpdated();
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnAbilityFinished();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	float Cooldown = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	bool IsGlobalCooldown = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	bool BlockMovementDuringUse = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	bool BlockShootingDuringUse = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Ability")
	bool BlockCastingDuringUse = false;
	
	UPROPERTY()
	UPlayerAbilitySystem* OwnerSystem;

	float CooldownRemaining = 0.0f;
	float DurationRemaining = 0.0f;
};
