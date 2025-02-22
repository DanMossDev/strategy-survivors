// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "GameFramework/Actor.h"
#include "Puddle.generated.h"

UCLASS()
class TOONTANKS_API APuddle : public AActor
{
	GENERATED_BODY()
	
public:	
	APuddle();
	UFUNCTION()
	void Init(EElementalType Type, float Scale);

	void HitByElement(EElementalType IncomingElement, bool ShouldRefresh);
	void AddToPuddle(EElementalType IncomingElement, float Scale);
	void SetTargetScale(float Scale);

protected:
	virtual void BeginPlay() override;
	void HandleDestroy();
	UFUNCTION(BlueprintImplementableEvent)
	void RefreshVisuals();

	void FireExplosion();

	class UPoolableComponent* Poolable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puddle", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EElementalType Element;
	UPROPERTY(EditAnywhere, Category = "Puddle")
	float Lifetime = 5.0f;

	float RemainingLifetime;
	float TargetScale;
	float CurrentScale;

	bool IsDying = false;

	//Interaction VFX
	UPROPERTY(EditAnywhere, Category = "Puddle")
	class UNiagaraSystem* FlameExplosion;

public:	
	virtual void Tick(float DeltaTime) override;
};
