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
	Oiled = 1 << 6 UMETA(DisplayName = "Oiled"),
};

ENUM_CLASS_FLAGS(EStatusEffect);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectComponent();

	void Init();

	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(EStatusEffect Effect, float Amount);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsSlowed() const {return HasStatusEffect(EStatusEffect::Slowed);}
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsStunned() const {return HasStatusEffect(EStatusEffect::Stunned);}
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsWet() const {return HasStatusEffect(EStatusEffect::Wet);}
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsFrozen() const {return HasStatusEffect(EStatusEffect::Frozen);}
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsBurning() const {return HasStatusEffect(EStatusEffect::Burning);}
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool IsOiled() const {return HasStatusEffect(EStatusEffect::Oiled);}

	void ClearAllEffects();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void UpdateStatusEffects(float DeltaTime);
	UFUNCTION()
	void DamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void AddEffect(EStatusEffect AddedEffect);
	void RemoveEffect(EStatusEffect RemovedEffect);

	UPROPERTY()
	class ABaseEntity* Entity;

	TMap<EStatusEffect, float> Effects;

	UPROPERTY(VisibleAnywhere, Category = "StatusEffects")
	EStatusEffect ActiveEffects;

	float TimeSinceLastBurnApplied = 0.0f;

	UPROPERTY(EditAnywhere, Category = "HealthComponent")
	FLinearColor DamageColor = FLinearColor::Red;
	UPROPERTY(EditAnywhere, Category = "HealthComponent")
	FLinearColor FrozenColor = FLinearColor::White;
	UPROPERTY(EditAnywhere, Category = "HealthComponent")
	FLinearColor WetColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, Category = "HealthComponent")
	FLinearColor BurningColor = FLinearColor(1.0f, 0.5f, 0.0f);
	UPROPERTY(EditAnywhere, Category = "HealthComponent")
	FLinearColor OiledColor = FLinearColor(0.5f, 0.0f, 1.0f);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool HasStatusEffect(EStatusEffect Effect) const;
};
