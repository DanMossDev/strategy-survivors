// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyMovementComponent();

	void Init();

	void SetOverrideDirection(FVector Destination, bool isTerrain);
	void ClearOverrideDestination();

protected:
	virtual void BeginPlay() override;

	bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StoppingDistance = 100.0f;

	FVector OverrideDirection;
	bool ShouldOverrideDirection = false;
	bool IsTerrain = false;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Move(float DeltaTime);
	bool MoveForward(float DeltaTime, float MovementSpeed);
	
	class AEnemy* Enemy;
};
