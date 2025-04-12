// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyLifetimeController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyLifetimeController : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyLifetimeController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnGetFromPool();
	
	UPROPERTY()
	class AEnemy* Enemy;
	UPROPERTY()
	class UEnemyMovementComponent* EnemyMovementComponent;
	UPROPERTY()
	class UPoolableComponent* PoolableComponent;

	float TimeAlive;
	bool RetreatStarted;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
