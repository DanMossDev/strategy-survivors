// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageNumber.generated.h"

UCLASS()
class TOONTANKS_API ADamageNumber : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageNumber();

	UFUNCTION(BlueprintCallable)
	void OnGetFromPool();
	UFUNCTION(BlueprintCallable)
	void OnReturnToPool();

	UFUNCTION(BlueprintImplementableEvent)
	void SetupBlueprint(float DamageToSet);
	
	void Init(float DamageToSet);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageNumber")
	class UWidgetComponent* WidgetComponent;
	class UPoolableComponent* Poolable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageNumber")
	float FloatUpSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageNumber")
	float Lifetime = 1.0f;

	FVector RandomMovement;
	float TimeAlive = 0.0f;
};
