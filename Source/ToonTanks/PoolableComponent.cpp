// Daniel Moss

//A component to be added to any actor you want to pool

#include "PoolableComponent.h"

UPoolableComponent::UPoolableComponent()
{
}

void UPoolableComponent::BeginPlay()
{
	Super::BeginPlay();

	PoolableActor = GetOwner();
}

void UPoolableComponent::GetFromPool()
{
	AActor* owner = GetOwner();
	owner->SetActorTickEnabled(true);
	owner->SetActorHiddenInGame(false);
	owner->SetActorEnableCollision(true);

	for (auto comp : owner->GetComponents())
	{
		comp->SetActive(true);
	}
	
	_isInPool = false;
	OnGetFromPool.Broadcast();
}

void UPoolableComponent::ReturnToPool()
{
	AActor* owner = GetOwner();
	owner->SetActorTickEnabled(false);
	owner->SetActorHiddenInGame(true);
	owner->SetActorEnableCollision(false);

	for (auto comp : owner->GetComponents())
	{
		comp->SetActive(false);
	}
	_isInPool = true;
	OnReturnToPool.Broadcast();
}

bool UPoolableComponent::IsAvailable() const
{
	return _isInPool;
}
