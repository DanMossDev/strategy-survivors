// Daniel Moss

//A Component to be added to GameMode or some other easily reached actor

#include "ObjectPoolComponent.h"

#include "PoolableComponent.h"

UObjectPoolComponent::UObjectPoolComponent()
{
}

void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TTuple<TSubclassOf<AActor>, int> startingPoolTuple : StartingPoolSizes)
	{
		TArray<UPoolableComponent*>* poolPtr = new TArray<UPoolableComponent*>();
		for (int i = 0; i < startingPoolTuple.Value; i++)
		{
			FActorSpawnParameters spawnParameters = FActorSpawnParameters();
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* newActor = GetWorld()->SpawnActor(startingPoolTuple.Key, nullptr, nullptr, spawnParameters);

			if (!newActor)
				return;
			
			UPoolableComponent* newPoolable = newActor->FindComponentByClass<UPoolableComponent>();
			if (!newPoolable)
				return;
			
			poolPtr->Add(newPoolable);
			newPoolable->ReturnToPool();
		}
		ObjectPools.Add(startingPoolTuple.Key, *poolPtr);
	}
}

UPoolableComponent* UObjectPoolComponent::GetFromPool(UClass* Class, FVector const &Location, FRotator const &Rotation)
{
	TArray<UPoolableComponent*>* poolPtr = ObjectPools.Find(Class);
	if (poolPtr == nullptr)
	{
		poolPtr = new TArray<UPoolableComponent*>();
		ObjectPools.Add(Class, *poolPtr);
		poolPtr = ObjectPools.Find(Class);
	}
	
	for (UPoolableComponent* actor : *poolPtr)
	{
		if (!actor->IsAvailable())
			continue;
	
		actor->PoolableActor->SetActorLocation(Location);
		actor->PoolableActor->SetActorRotation(Rotation);
		actor->GetFromPool();
		return actor;
	}
	FActorSpawnParameters spawnParameters = FActorSpawnParameters();
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* newActor = GetWorld()->SpawnActor(Class, &Location, &Rotation, spawnParameters);

	if (!newActor)
		return nullptr;
	
	UPoolableComponent* newPoolable = newActor->FindComponentByClass<UPoolableComponent>();
	if (!newPoolable)
		return nullptr;
	
	poolPtr->Add(newPoolable);
	newPoolable->GetFromPool();
	return newPoolable;
}
