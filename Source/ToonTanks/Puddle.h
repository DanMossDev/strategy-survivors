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
	void Init();

	void HitByElement(EElementalType IncomingElement);

protected:
	virtual void BeginPlay() override;
	void HandleDestroy();
	UFUNCTION(BlueprintImplementableEvent)
	void RefreshVisuals();

	class UPoolableComponent* Poolable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puddle", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puddle", meta = (AllowPrivateAccess = "true"))
	EElementalType Element;
	UPROPERTY(EditAnywhere, Category = "Puddle")
	float Lifetime = 5.0f;

	float RemainingLifetime;

public:	
	virtual void Tick(float DeltaTime) override;
};
