// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "BaseEntity.h"
#include "EntityStats.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::Init(UEntityStats* EntityStats)
{
	CurrentHealth = EntityStats->MaxHealth;
	IsDead = false;
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead) return;
	
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0.0f)
	{
		IsDead = true;
		if (ABaseEntity* entity = Cast<ABaseEntity>(DamagedActor))
			entity->OnDeath();
	}
}

