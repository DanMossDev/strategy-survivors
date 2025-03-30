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
	virtual void CheckInRange() override;

	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	TSubclassOf<class AColumnOfFire> FireColumnClass;

	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	int32 NumberOfFlames = 5;
	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	float CastDuration = 2.0f;

	void ProcessAttackVariantRing(float DeltaTime);
	void ProcessAttackVariantLine(float DeltaTime);

	void BeginAttackVariantRing();

	int32 AttackVariant = 0;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginAttack() override;
	virtual void ProcessAttack(float DeltaTime) override;
	virtual void OnOwnerDeath() override;
};
