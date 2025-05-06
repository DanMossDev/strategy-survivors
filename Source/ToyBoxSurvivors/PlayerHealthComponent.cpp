// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthComponent.h"

#include "EntityStats.h"
#include "Tank.h"

UPlayerHealthComponent::UPlayerHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ProgressToHeal = 0.0f;
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<ATank>(GetOwner());
}


void UPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!EntityStats)
		return;

	ProgressToHeal += DeltaTime * EntityStats->GetHealthRegen();

	if (ProgressToHeal > 1.0f)
	{
		Heal(1.0f);
		ProgressToHeal = 0.0f;
	}
}

void UPlayerHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	
	ProgressToHeal = 0.0f;
	Player->ShowDamageTaken();
	OnPlayerHealthChanged.Broadcast();
}

void UPlayerHealthComponent::Heal(float Health)
{
	Super::Heal(Health);

	OnPlayerHealthChanged.Broadcast();
}