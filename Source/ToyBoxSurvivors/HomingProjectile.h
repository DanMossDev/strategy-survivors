// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HomingProjectile.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHomingProjectile : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHomingProjectile();

	void Init();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CheckForHoming();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingProjectile")
	float HomingRange = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingProjectile")
	float HomingAcceleration = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HomingProjectile")
	float StartingTime = 0.25f;

	float Lifetime = 0.0f;

	class UProjectileMovementComponent* ProjectileMovement;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
