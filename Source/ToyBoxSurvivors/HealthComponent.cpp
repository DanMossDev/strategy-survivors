// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "BaseEntity.h"
#include "DamageNumber.h"
#include "EntityStats.h"
#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::Init(UEntityStats*Stats)
{
	EntityStats = Stats;
	CurrentHealth = EntityStats->GetMaxHealth();
	IsDead = false;
}

void UHealthComponent::TakeDamageManual(int32 Damage)
{
	if (IsInvincible || Damage <= 0.0f || IsDead) return;
	
	CurrentHealth -= Damage;

	auto damageNumber = GameMode->GetObjectPool()->GetFromPool<ADamageNumber>(GameMode->GetDamageNumberClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	damageNumber->Init(Damage);
	
	if (CurrentHealth <= 0.0f)
	{
		IsDead = true;
		if (ABaseEntity* entity = Cast<ABaseEntity>(GetOwner()))
			entity->OnDeath();
		return;
	}
	
	if (EntityStats->GetHitInvincibilityTime() > 0.0f)
	{
		IsInvincible = true;
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			if (IsValid(this))
				IsInvincible = false;
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, EntityStats->GetHitInvincibilityTime(), false);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsInvincible || Damage <= 0.0f || IsDead) return;
	
	CurrentHealth -= Damage;

	auto damageNumber = GameMode->GetObjectPool()->GetFromPool<ADamageNumber>(GameMode->GetDamageNumberClass(), DamagedActor->GetActorLocation(), DamagedActor->GetActorRotation());
	damageNumber->Init(Damage);
	
	if (CurrentHealth <= 0.0f)
	{
		IsDead = true;
		if (ABaseEntity* entity = Cast<ABaseEntity>(DamagedActor))
			entity->OnDeath();
		return;
	}
	
	if (EntityStats->GetHitInvincibilityTime() > 0.0f)
	{
		IsInvincible = true;
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			if (IsValid(this))
				IsInvincible = false;
		});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, EntityStats->GetHitInvincibilityTime(), false);
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
