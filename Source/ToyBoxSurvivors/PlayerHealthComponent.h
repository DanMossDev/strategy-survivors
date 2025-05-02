// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "PlayerHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHealthChanged);

UCLASS()
class TOONTANKS_API UPlayerHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

public:
	UPlayerHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

	virtual void Heal(float amount) override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerHealthChanged OnPlayerHealthChanged;

protected:
	virtual void BeginPlay() override;
	float ProgressToHeal;

	UPROPERTY()
	class ATank* Player;
};
