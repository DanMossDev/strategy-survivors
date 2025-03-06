// Daniel Moss

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UPoolableComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetFromPool);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReturnToPool);

public:	
	UPoolableComponent();

	void GetFromPool();
	void ReturnToPool();

	AActor* PoolableActor;

	bool IsAvailable() const;
	UPROPERTY(BlueprintAssignable)
	FGetFromPool OnGetFromPool;
	UPROPERTY(BlueprintAssignable)
	FReturnToPool OnReturnToPool;

protected:
	virtual void BeginPlay() override;
	
	bool _isInPool;
};
