// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitingProjectile.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UOrbitingProjectile : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrbitingProjectile();
	
	void Init(bool ShotAlternator);

protected:
	virtual void BeginPlay() override;

	void RotateProjectile(float DeltaTime);

	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitingProjectile")
	bool DecayingOrbit = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitingProjectile")
	bool AlternateDirection = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitingProjectile")
	float StartingTime = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitingProjectile")
	float RotationRate = 1.0f;
	float Lifetime;
	bool ShotAlternated;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
