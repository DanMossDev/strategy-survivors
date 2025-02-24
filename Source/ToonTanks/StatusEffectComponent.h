// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EStatusEffect : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Slowed = 1 << 1 UMETA(DisplayName = "Slowed"),
	Stunned = 1 << 2 UMETA(DisplayName = "Stunned"),
	Wet = 1 << 3 UMETA(DisplayName = "Wet"),
	Frozen = 1 << 4 UMETA(DisplayName = "Frozen"),
	Burning = 1 << 5 UMETA(DisplayName = "Burning"),
	Poisoned = 1 << 6 UMETA(DisplayName = "Poisoned"),
};

ENUM_CLASS_FLAGS(EStatusEffect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

	void Init(class ABaseEntity* entity);

	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(EStatusEffect Effect, float Amount);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void UpdateStatusEffects(float DeltaTime);

	ABaseEntity* Entity;

	TMap<EStatusEffect, float> Effects;

	UPROPERTY(VisibleAnywhere, Category = "StatusEffects")
	EStatusEffect ActiveEffects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool HasStatusEffect(EStatusEffect Effect);
};
