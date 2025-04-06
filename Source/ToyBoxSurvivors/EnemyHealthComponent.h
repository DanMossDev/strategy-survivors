// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "EnemyHealthComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UEnemyHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
};
