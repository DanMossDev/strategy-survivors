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
	owner->SetActorEnableCollision(true);

	for (auto comp : owner->GetComponents())
	{
		comp->SetComponentTickEnabled(true);
		comp->SetActive(true);
	}
	
	owner->SetActorHiddenInGame(false);
	_isInPool = false;
	OnGetFromPool.Broadcast();
}

void UPoolableComponent::ReturnToPool()
{
	AActor* owner = GetOwner();
	owner->SetActorHiddenInGame(true);
	owner->SetActorTickEnabled(false);
	owner->SetActorEnableCollision(false);

	for (auto comp : owner->GetComponents())
	{
		comp->SetComponentTickEnabled(false);
		comp->SetActive(false);
	}
	_isInPool = true;
	OnReturnToPool.Broadcast();
}

bool UPoolableComponent::IsAvailable() const
{
	return _isInPool;
}
