// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolableComponent.h"
#include "GameFramework/Actor.h"
#include "AttackAnticipationIndicator.generated.h"

UCLASS()
class TOONTANKS_API AAttackAnticipationIndicator : public AActor
{
	GENERATED_BODY()

public:
	AAttackAnticipationIndicator();

	virtual void UpdateAnticipation(const FVector& Scale, const FVector& OwnerLocation, const FRotator& OwnerRotation);

	void ReturnToPool() const {Poolable->ReturnToPool();}
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnGetFromPool();
	UFUNCTION()
	void OnReturnToPool();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anticipation", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY()
	class UPoolableComponent* Poolable;
};
