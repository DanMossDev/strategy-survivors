// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UEnemyAttackController : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyAttackController();

protected:
	virtual void BeginPlay() override;

	class AEnemy* Enemy;
	TArray<class UEnemyAttack*> AttackOptions;

	bool HasComponents = false;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
