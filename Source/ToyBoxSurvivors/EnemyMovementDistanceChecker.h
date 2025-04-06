// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyMovementDistanceChecker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyMovementDistanceChecker : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyMovementDistanceChecker();

protected:
	virtual void BeginPlay() override;

	class UEnemyMovementComponent* EnemyMovementComponent;
	class UCapsuleComponent* EnemyCollision;
	
	bool GetClosestActorInRange(FVector& OverrideDirection, bool& IsTerrain);

	UPROPERTY(EditAnywhere, Category = "Enemy Movement")
	float AvoidObjectsDistance;

	UPROPERTY(EditAnywhere, Category = "Enemy Movement")
	bool AvoidOtherEnemies = true;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
