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

	UPROPERTY(EditAnywhere, Category = "Attack Telegraphing")
	TSubclassOf<class AColumnOfFire> FireColumnClass;

	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	int32 NumberOfFlames = 5;
	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	float TelegraphTime = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	float FlameDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	float DamageMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	float LocationOffset = 250.0f;
	UPROPERTY(EditAnywhere, Category = "Fire Spell")
	FVector ColumnSize = FVector(3,3,5);

	void ProcessAttackVariantRing(float DeltaTime);
	void ProcessAttackVariantLine(float DeltaTime);

	void BeginAttackVariantRing();

	int32 AttackVariant = 0;

public:
	virtual void CheckInRange() override;
	virtual void BeginAttack() override;
	virtual void ProcessAttack(float DeltaTime) override;
	virtual void OnOwnerDeath() override;
};
