// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEntity.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	ATank();

	virtual void OnDeath() override;

	APlayerController* GetPlayerController() const;

	void AddStatBoost(class UStatBoost* StatBoost);
	void Heal(int32 amount);

protected:
	virtual void BeginPlay() override;

	virtual float GetCurrentMovementSpeed() const override;

	TMap<FString, int32> CollectedStatBoosts;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class URotatingTurretComponent* TurretComponent;

	UPROPERTY(EditAnywhere)
	class UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere)
	UInputAction* ShootInputAction;
	struct FEnhancedInputActionValueBinding* MoveActionValueBinding;

	UPROPERTY(EditAnywhere, Category = "Tank")
	TSubclassOf<UCameraShakeBase> HitCameraShake; //TODO - implement this

	UPROPERTY(EditAnywhere, Category = "Tank")
	class UWeaponInfo* StarterWeapon;

	FVector2D MoveInput;
	bool ShootHeld;
	float TimeSinceLastShot;

	UFUNCTION(BlueprintCallable)
	FVector2D GetMoveInput() const;

	class AToonTanksPlayerController* PlayerController;

	float CachedMovement;

	UFUNCTION()
	void Move(const struct FInputActionValue& Value);

	void ProcessMovement(float DeltaTime);
	void ProcessTurretRotation(float DeltaTime);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;
};
