// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "BaseEntity.h"
#include "EntityStats.h"

UStatusEffectComponent::UStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Effects.Add(EStatusEffect::Slowed, 0);
	Effects.Add(EStatusEffect::Stunned, 0);
	Effects.Add(EStatusEffect::Wet, 0);
	Effects.Add(EStatusEffect::Frozen, 0);
	Effects.Add(EStatusEffect::Burning, 0);
	Effects.Add(EStatusEffect::Poisoned, 0);
}

void UStatusEffectComponent::Init(ABaseEntity* entity)
{
	ActiveEffects = EStatusEffect::None;

	Entity = entity;
	if (Entity)
		Entity->EntityStats->InjectStatusEffectComponent(this);
}

void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStatusEffects(DeltaTime);
}

void UStatusEffectComponent::UpdateStatusEffects(float DeltaTime)
{
	for (auto& kvp : Effects)
	{
		float& value = kvp.Value;
		value -= DeltaTime;

		if (value <= 0)
		{
			value = 0;
			ActiveEffects &= ~kvp.Key;
		}

		if (value > 0)
		{
			ActiveEffects |= kvp.Key;
		}
	}
}

void UStatusEffectComponent::AddStatusEffect(EStatusEffect Effect, float Amount)
{
	float* amount = Effects.Find(Effect);
	*amount += Amount;
}

bool UStatusEffectComponent::HasStatusEffect(EStatusEffect Effect) const
{
	return EnumHasAnyFlags(ActiveEffects, Effect);
}