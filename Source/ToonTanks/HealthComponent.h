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
	// Sets default values for this component's properties
	UHealthComponent();
	
	void Init(class UEntityStats* Stats);

	bool IsDead = false;

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Heal(float amount);

	UFUNCTION(BlueprintCallable)
	float GetMaxHP() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentHP() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UEntityStats* EntityStats;

	bool IsInvincible = false;

private:
	float CurrentHealth = 0.0f;
};
