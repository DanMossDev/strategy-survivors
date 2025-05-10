// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BouncingBullets.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UBouncingBullets : public UActorComponent
{
	GENERATED_BODY()

public:
	UBouncingBullets();

	void Init();
	void OnHitEnemy(const AActor* EnemyHit);

protected:
	virtual void BeginPlay() override;

	void AcquireNewTarget();

	UPROPERTY()
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Bouncing Bullets")
	float DistanceToCheckForNewTarget = 1500.0f;

	FCollisionQueryParams QueryParams;
};
