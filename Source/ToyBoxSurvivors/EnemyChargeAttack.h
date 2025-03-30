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
	virtual void CheckInRange() override;
	
	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	float TelegraphTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	float ChargeTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	float ChargeSpeed = 500.0f;

	FVector HitIndicatorScale;
	float AttackDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	TSubclassOf<class AAttackAnticipationIndicator> AnticipationIndicatorClass;
	
	UPROPERTY()
	AAttackAnticipationIndicator* AnticipationIndicator;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginAttack() override;
	virtual void ProcessAttack(float DeltaTime) override;
};
