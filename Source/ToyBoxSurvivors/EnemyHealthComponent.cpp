// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthComponent.h"

#include "EventDispatcher.h"

void UEnemyHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	UEventDispatcher::EnemyReceivedDamage(DamageType, Damage);
}
