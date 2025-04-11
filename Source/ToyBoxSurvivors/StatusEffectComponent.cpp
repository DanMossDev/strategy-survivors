// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"

#include "BaseEntity.h"
#include "EntityStats.h"
#include "FireDamage.h"
#include "IceDamage.h"
#include "OilDamage.h"
#include "PhysicalDamage.h"
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

	Entity = Cast<ABaseEntity>(GetOwner());
	if (!Entity)
		return;
	
	Entity->EntityStats->InjectStatusEffectComponent(this);
	
	Entity->OnTakeAnyDamage.AddDynamic(this, &UStatusEffectComponent::DamageReceived);
}

void UStatusEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Entity->OnTakeAnyDamage.IsAlreadyBound(this, &UStatusEffectComponent::DamageReceived))
		Entity->OnTakeAnyDamage.RemoveDynamic(this, &UStatusEffectComponent::DamageReceived);
}


void UStatusEffectComponent::Init()
{
	TimeSinceLastBurnApplied = 0.0f;
	ClearAllEffects();
}

void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateStatusEffects(DeltaTime);
}

void UStatusEffectComponent::DamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageType->IsA(UPhysicalDamage::StaticClass()))
	{
		Entity->SetOverlayColor(DamageColor);
		return;
	}
	
	if (DamageType->IsA(UFireDamage::StaticClass()))
	{
		//Handle Fire
		AddStatusEffect(EStatusEffect::Burning, 2.0f);

		Entity->SetOverlayColor(BurningColor);
		return;
	}

	if (DamageType->IsA(UWaterDamage::StaticClass()))
	{
		//Handle Water
		AddStatusEffect(EStatusEffect::Wet, 3.0f);

		Entity->SetOverlayColor(WetColor);
		return;
	}
	
	if (DamageType->IsA(UIceDamage::StaticClass()))
	{
		//Handle Ice
		if (HasStatusEffect(EStatusEffect::Wet))
		{
			AddStatusEffect(EStatusEffect::Frozen, 1.0f);
			Entity->SetOverlayColor(FrozenColor);
		}
		else
		{
			AddStatusEffect(EStatusEffect::Wet, 1.0f);
			Entity->SetOverlayColor(WetColor);
		}
		return;
	}
	
	if (DamageType->IsA(UOilDamage::StaticClass()))
	{
		//Handle Oil
		AddStatusEffect(EStatusEffect::Oiled, 3.0f);

		Entity->SetOverlayColor(OiledColor);

		return;
	}

	Entity->SetOverlayColor(DamageColor);
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
			RemoveEffect(kvp.Key);
		}

		if (value > 0)
		{
			AddEffect(kvp.Key);

			if (kvp.Key == EStatusEffect::Burning)
			{
				if (TimeSinceLastBurnApplied > 0.95f)
				{
					Entity->TakeFireDamage(HasStatusEffect(EStatusEffect::Oiled));
					TimeSinceLastBurnApplied = 0.0f;
				}
				else
					TimeSinceLastBurnApplied += DeltaTime;
			}
		}
	}
}

void UStatusEffectComponent::AddEffect(EStatusEffect AddedEffect)
{
	if (HasStatusEffect(AddedEffect))
		return;
	ActiveEffects |= AddedEffect;
	Entity->OnUpdateStatusEffectUI();
}

void UStatusEffectComponent::RemoveEffect(EStatusEffect RemovedEffect)
{
	if (!HasStatusEffect(RemovedEffect))
		return;
	if (RemovedEffect == EStatusEffect::Burning)
		TimeSinceLastBurnApplied = 0.0f;
	
	ActiveEffects &= ~RemovedEffect;
	Entity->OnUpdateStatusEffectUI();
}

void UStatusEffectComponent::AddStatusEffect(EStatusEffect Effect, float Amount)
{
	float* amount = Effects.Find(Effect);
	AddEffect(Effect);

	if (amount && *amount < Amount)
	{
		*amount = Amount;
	}
}

bool UStatusEffectComponent::HasStatusEffect(EStatusEffect Effect) const
{
	return EnumHasAnyFlags(ActiveEffects, Effect);
}

void UStatusEffectComponent::ClearAllEffects()
{
	for (auto& kvp : Effects)
	{
		float& value = kvp.Value;
		value = 0;
	}
	ActiveEffects = EStatusEffect::None;
	
	Entity->OnUpdateStatusEffectUI();
}
