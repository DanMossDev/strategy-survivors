// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEntity.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AEnemy : public ABaseEntity
{
	GENERATED_BODY()

public:
	AEnemy();

	UFUNCTION(BlueprintCallable)
	void OnGetFromPool();
	UFUNCTION(BlueprintCallable)
	void OnReturnToPool();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyGotFromPool();

	void OverrideEntityStats(UEntityStats* OverrideWith);
	
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath() override;

	AActor* TargetActor;
	class UEnemyMovementComponent* MovementComponent;
	
protected:
	virtual void BeginPlay() override;
	void SpawnRandomPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	class UPoolableComponent* PoolableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	TMap<float, TSubclassOf<class APickup>> WeightedPickupPool;
};
