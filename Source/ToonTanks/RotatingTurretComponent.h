// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RotatingTurretComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API URotatingTurretComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatingTurretComponent();

	void RotateTurret(const FVector& TargetLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RotatingTurret", meta = (AllowPrivateAccess = "true"))
	// UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, Category = "RotatingTurret")
	float TurretRotationSpeed = 180.0f;
	
	UPROPERTY(EditAnywhere, Category = "RotatingTurret")
	float MinimumDistanceToAllowRotate = 100.0f;
};
