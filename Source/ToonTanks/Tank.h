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
	// Sets default values for this pawn's properties
	ATank();

	virtual void OnDeath() override;

	APlayerController* GetPlayerController() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 100.0f;

	FVector2D MoveInput;
	bool ShootHeld;
	float TimeSinceLastShot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	float ShotDelay = 0.5f;

	UFUNCTION(BlueprintCallable)
	FVector2D GetMoveInput() const;

	class AToonTanksPlayerController* PlayerController;

	UFUNCTION()
	void Move(const struct FInputActionValue& Value);
	UFUNCTION()
	void Shoot(const FInputActionValue& Value);

	void ProcessMovement(float DeltaTime);
	void ProcessTurretRotation(float DeltaTime);
	void ProcessShoot(float DeltaTime);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
