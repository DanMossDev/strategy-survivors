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

public:
	virtual void ProcessAttack(float DeltaTime) {}
	virtual void OnOwnerDeath() {}
};
