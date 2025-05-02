// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	
	void Init(class UEntityStats* Stats);

	bool IsDead = false;

	void TakeDamageManual(int32 Amount);

	bool GetIsInvincible() const {return IsInvincible;}

	UFUNCTION()
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	virtual void Heal(float amount);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHP() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	class ABaseEntity* Entity;
	UPROPERTY()
	UEntityStats* EntityStats;

	bool IsInvincible = false;

	bool IsPlayer = false;

	UPROPERTY()
	class AToonTanksGameMode* GameMode;

private:
	float CurrentHealth = 0.0f;
};
