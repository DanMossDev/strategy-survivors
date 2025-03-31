// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyAttack();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class AEnemy* Enemy;
	UPROPERTY()
	class AToonTanksGameMode* GameMode;
	UPROPERTY()
    class UEnemyMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float AttackRange = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float Cooldown = 5.0f;

	float AttackTime = 0.0f;


public:
	virtual void CheckInRange() {}
	virtual void BeginAttack() {}
	virtual void ProcessAttack(float DeltaTime) {}
	virtual void OnOwnerDeath() {}
};
