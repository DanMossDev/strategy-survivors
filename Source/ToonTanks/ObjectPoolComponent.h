// Daniel Moss

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.h"
#include "ObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolComponent();

	UPoolableComponent* GetFromPool(UClass* Class, FVector const &Location, FRotator const &Rotation);
	
	template<class T>
	T* GetFromPool(UClass* Class, FVector const &Location, FRotator const &Rotation)
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
			return CastChecked<T>(actor->PoolableActor,ECastCheckedType::NullAllowed);
		}
	
		AActor* newActor = GetWorld()->SpawnActor(Class, &Location, &Rotation);
		UPoolableComponent* newPoolable = newActor->FindComponentByClass<UPoolableComponent>();
		poolPtr->Add(newPoolable);
		newPoolable->GetFromPool();
		return CastChecked<T>(newPoolable->PoolableActor,ECastCheckedType::NullAllowed);
	}

protected:
	virtual void BeginPlay() override;

	TMap<TSubclassOf<AActor>, TArray<UPoolableComponent*>> ObjectPools;
	TArray<UPoolableComponent*> ProjectilePool = TArray<UPoolableComponent*>();

	UPROPERTY(EditAnywhere, Category="Object Pool")
	TMap<TSubclassOf<AActor>, int> StartingPoolSizes;
};
