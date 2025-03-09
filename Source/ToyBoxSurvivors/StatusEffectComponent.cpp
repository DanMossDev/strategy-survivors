// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "BaseEntity.h"
#include "EntityStats.h"
#include "FireDamage.h"
#include "IceDamage.h"
#include "OilDamage.h"
#include "WaterDamage.h"

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
	Effects.Add(EStatusEffect::Oiled, 0);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatusEffectComponent::DamageReceived);
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

void UStatusEffectComponent::DamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageType->IsA(UWaterDamage::StaticClass()))
	{
		//Handle Water
		AddStatusEffect(EStatusEffect::Wet, 10.0f);
	}
	else if (DamageType->IsA(UFireDamage::StaticClass()))
	{
		//Handle Fire
		AddStatusEffect(EStatusEffect::Burning, 10.0f);
	}
	else if (DamageType->IsA(UIceDamage::StaticClass()))
	{
		//Handle Ice
		AddStatusEffect(EStatusEffect::Frozen, 10.0f);
	}
	else if (DamageType->IsA(UOilDamage::StaticClass()))
	{
		//Handle Oil
		AddStatusEffect(EStatusEffect::Oiled, 10.0f);
	}
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

	if (amount)
	{
		*amount += Amount;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO STATUS EFFECT COMP ON %s"), *GetOwner()->GetName());
	}
}

bool UStatusEffectComponent::HasStatusEffect(EStatusEffect Effect) const
{
	return EnumHasAnyFlags(ActiveEffects, Effect);
}