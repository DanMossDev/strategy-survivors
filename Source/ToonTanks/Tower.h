// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEntity.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABaseEntity
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTowerDestroyed);

public:
	ATower();

	UFUNCTION(BlueprintCallable)
	void OnGetFromPool();
	UFUNCTION(BlueprintCallable)
	void OnReturnToPool();
	
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath() override;

	static FTowerDestroyed OnTowerDestroyed;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower", meta = (AllowPrivateAccess = "true"))
	class UPoolableComponent* PoolableComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower", meta = (AllowPrivateAccess = "true"))
	class URotatingTurretComponent* TurretComponent;
	
private:
	bool CheckDistance(const FVector& TargetLocation) const;

	AActor* TargetActor;
	FTimerHandle FireRateTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Tower")
	float AttackRange = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Tower")
	float FireRate = 2.0f;

	void CheckFireCondition();

	UFUNCTION()
	void OnGameOver();
};
