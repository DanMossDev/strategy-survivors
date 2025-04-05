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

	class UInventory* GetInventory() const {return Inventory;}

	void AddStatBoost(class UStatBoost* StatBoost);
	void Heal(int32 amount);

	void SetManualAim(const bool Aim) {ManualAim = Aim;}

	void ToggleManualAim() {ManualAim = !ManualAim;}

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	virtual float GetCurrentMovementSpeed() const override;

	TMap<FString, int32> CollectedStatBoosts;

	bool ManualAim = false;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class URotatingTurretComponent* TurretComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UInventory* Inventory;

	UPROPERTY(EditAnywhere)
	class UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere)
	class UInputAction* AimInputAction;
	UPROPERTY(EditAnywhere)
	UInputAction* ToggleManualAimInputAction;
	struct FEnhancedInputActionValueBinding* MoveActionValueBinding;

	UPROPERTY(EditAnywhere, Category = "Tank")
	TSubclassOf<UCameraShakeBase> HitCameraShake; //TODO - implement this

	UPROPERTY(EditAnywhere, Category = "Tank")
	TArray<class UWeaponInfo*> StarterWeapons;

	FVector2D MoveInput;
	FVector2D AimInput;
	float TimeSinceLastShot;

	UFUNCTION(BlueprintCallable)
	FVector2D GetMoveInput() const;

	class AToonTanksPlayerController* PlayerController;

	float CachedMovement;

	UFUNCTION()
	void Move(const struct FInputActionValue& Value);
	UFUNCTION()
	void Aim(const struct FInputActionValue& Value);
	UFUNCTION()
	void ToggleManualAimInput(const FInputActionValue& Value);

	void ProcessMovement(float DeltaTime);
	void ProcessTurretRotation(float DeltaTime);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	virtual void Tick(float DeltaTime) override;
};
