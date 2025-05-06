// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "BaseEntity.h"
#include "DamageNumber.h"
#include "EntityStats.h"
#include "ObjectPoolComponent.h"
#include "Tank.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	Entity = Cast<ABaseEntity>(GetOwner());
	Entity->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	IsPlayer = Entity->IsA(ATank::StaticClass());
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Entity->OnTakeAnyDamage.IsAlreadyBound(this, &UHealthComponent::TakeDamage))
		Entity->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::Init(UEntityStats* Stats)
{
	EntityStats = Stats;
	CurrentHealth = EntityStats->GetMaxHealth();
	IsDead = false;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InvincibleTime > 0.0f)
	{
		InvincibleTime -= DeltaTime;
	}
}

void UHealthComponent::TakeDamageManual(int32 Damage)
{
	if (GetIsInvincible() || Damage <= 0.0f || IsDead) return;
	
	CurrentHealth -= Damage;

	auto damageNumber = GameMode->GetObjectPool()->GetFromPool<ADamageNumber>(GameMode->GetDamageNumberClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	damageNumber->Init(Damage, IsPlayer);
	
	if (CurrentHealth <= 0.0f)
	{
		IsDead = true;
		if (ABaseEntity* entity = Cast<ABaseEntity>(GetOwner()))
			entity->OnDeath();
		return;
	}
	
	if (EntityStats->GetHitInvincibilityTime() > 0.0f)
	{
		SetInvincibleForTime(EntityStats->GetHitInvincibilityTime());
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetIsInvincible() || Damage <= 0.0f || IsDead) return;
	
	CurrentHealth -= Damage;

	auto damageNumber = GameMode->GetObjectPool()->GetFromPool<ADamageNumber>(GameMode->GetDamageNumberClass(), DamagedActor->GetActorLocation(), DamagedActor->GetActorRotation());
	damageNumber->Init(Damage, IsPlayer);
	
	if (CurrentHealth <= 0.0f)
	{
		IsDead = true;
		Entity->OnDeath();
		return;
	}

	Entity->BeginHitReact();
	
	if (EntityStats->GetHitInvincibilityTime() > 0.0f)
	{
		SetInvincibleForTime(EntityStats->GetHitInvincibilityTime());
	}
}

void UHealthComponent::Heal(float amount)
{
	CurrentHealth += amount;
	if (CurrentHealth > EntityStats->GetMaxHealth())
		CurrentHealth = EntityStats->GetMaxHealth();
}


float UHealthComponent::GetMaxHP() const
{
	return EntityStats->GetMaxHealth();
}

float UHealthComponent::GetCurrentHP() const
{
	return CurrentHealth;
}

void UHealthComponent::SetInvincibleForTime(float Duration)
{
	if (InvincibleTime < Duration)
		InvincibleTime = Duration;
}
