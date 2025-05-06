// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerAbilitySystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UPlayerAbilitySystem : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPlayerAbilitySystem();

	void InitialiseActionMappings(UEnhancedInputComponent* EnhancedInputComponent);

	void RegisterAbility(class UPlayerAbility* Ability, int Slot);

	void SetGlobalCooldown(float Cooldown);
	
	float GetRemainingGlobalCooldown() const;

	bool IsMovementLocked() const;
	bool IsShootingLocked() const;
	bool IsCastingLocked() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	float MostRecentGlobalCooldown = 0.0f;
	UPROPERTY()
	float GlobalCooldownRemaining = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Player Ability System")
	class UInputAction* Ability1InputAction;
	UPROPERTY(EditAnywhere, Category = "Player Ability System")
	UInputAction* Ability2InputAction;
	UPROPERTY(EditAnywhere, Category = "Player Ability System")
	UInputAction* Ability3InputAction;
	UPROPERTY(EditAnywhere, Category = "Player Ability System")
	UInputAction* Ability4InputAction;

	UPROPERTY()
	TArray<UPlayerAbility*> PlayerAbilities;

	void CastAbility1(const struct FInputActionValue& Value);
	void CastAbility2(const FInputActionValue& Value);
	void CastAbility3(const FInputActionValue& Value);
	void CastAbility4(const FInputActionValue& Value);

public:
	virtual void TickComponent(float DeltaTime,  ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
