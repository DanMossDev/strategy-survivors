// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttack.h"
#include "Components/ActorComponent.h"
#include "EnemyFireSpell.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyFireSpell : public UEnemyAttack
{
	GENERATED_BODY()

public:	
	UEnemyFireSpell();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// virtual void ProcessAttack(float DeltaTime) override;
	// virtual void OnOwnerDeath() override;
};
