// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyChargeAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyChargeAttack : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyChargeAttack();

	void Init();

	UFUNCTION()
	void OnOwnerDeath();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CheckInRange();
	void ProcessAttack(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Charge Attack")
	TSubclassOf<class AAttackAnticipationIndicator> AnticipationIndicatorClass;

	class AToonTanksGameMode* GameMode;

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
	float CooldownRemaining = 0.0f;

	bool Attacking = false;

	FVector HitIndicatorScale;
	float AttackDistance = 0.0f;

	AAttackAnticipationIndicator* AnticipationIndicator;
	class UEnemyMovementComponent* MovementComponent;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class AEnemy* Enemy;
};
