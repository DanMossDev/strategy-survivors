// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColumnOfFire.generated.h"

UCLASS()
class TOONTANKS_API AColumnOfFire : public AActor
{
	GENERATED_BODY()
	
public:
	AColumnOfFire();

	void Init(const FVector& Scale, float TelegraphTime, float AttackTime, float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void CheckForPlayer();
	
	UFUNCTION()
	void OnGetFromPool();
	UFUNCTION()
	void OnReturnToPool();

	UPROPERTY(EditAnywhere, Category = "Column of Fire")
	UMaterial* TelegraphingMaterial;
	UPROPERTY(EditAnywhere, Category = "Column of Fire")
	UMaterial* FireMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Column of Fire")
	UStaticMeshComponent* Mesh;
	FVector TargetScale;
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "Column of Fire")
	class UPoolableComponent* Poolable;

	bool ShowingFire = false;
	float TimeAlive = 0.0f;
	float TelegraphDuration = 0.0f;
	float AttackDuration = 0.0f;

	float OutgoingDamage = 0.0f;

public:
	virtual void Tick(float DeltaTime) override;
};
