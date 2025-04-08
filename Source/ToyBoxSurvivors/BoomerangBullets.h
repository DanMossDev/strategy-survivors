// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoomerangBullets.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UBoomerangBullets : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBoomerangBullets();

	void Init();

protected:
	virtual void BeginPlay() override;

	FVector DirectionToReturn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Boomerang")
	float BoomerangSpeed = 1000.0f;

	class AProjectile* Projectile;
	class UProjectileMovementComponent* ProjectileMovement;

	bool HasFlipped = false;
	int32 StartX;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
