// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEntity.h"
#include "EntityStats.h"
#include "Components/CapsuleComponent.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AEnemy : public ABaseEntity
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

public:
	AEnemy();

	FOnDeath OnDeathEvent;

	UFUNCTION(BlueprintCallable)
	void OnGetFromPool();
	UFUNCTION(BlueprintCallable)
	void OnReturnToPool();

	void BeginAttack(class UEnemyAttack* AttackToBegin);
	void FinishAttack(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyGotFromPool();

	void OverrideEntityStats(UEntityStats* OverrideWith);

	float GetCollisionWidth() const {return CapsuleComponent->GetScaledCapsuleRadius() * 2.0f;}

	float GetSpellDamage() const {return EntityStats->GetContactDamageAmount();}
	
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath() override;

	UPROPERTY()
	AActor* TargetActor;
	UPROPERTY()
	class UEnemyMovementComponent* MovementComponent;

	float CooldownRemaining;
	bool IsAttacking;
	
protected:
	virtual void BeginPlay() override;
	void SpawnRandomPickup();

	UPROPERTY()
	UEnemyAttack* Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	class UEnemyAttackController* AttackController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	class UPoolableComponent* PoolableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	TMap<float, TSubclassOf<class APickup>> WeightedPickupPool;
};
