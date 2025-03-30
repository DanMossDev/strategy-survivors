// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttack.h"
#include "Components/ActorComponent.h"
#include "EnemyChargeAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyChargeAttack : public UEnemyAttack
{
	GENERATED_BODY()

public:	
	UEnemyChargeAttack();

	void Init();

	virtual void OnOwnerDeath() override;

protected:
	virtual void BeginPlay() override;

	bool CheckInRange();

	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	TSubclassOf<class AAttackAnticipationIndicator> AnticipationIndicatorClass;

	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float AttackRange = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float TelegraphTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float ChargeTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float ChargeSpeed = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	float Cooldown = 5.0f;
	
	float AttackTime = 0.0f;

	FVector HitIndicatorScale;
	float AttackDistance = 0.0f;

	UPROPERTY()
	AAttackAnticipationIndicator* AnticipationIndicator;
	UPROPERTY()
	class UEnemyMovementComponent* MovementComponent;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ProcessAttack(float DeltaTime) override;
};
