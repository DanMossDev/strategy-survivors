// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	None UMETA(DisplayName = "None"),
	XP UMETA(DisplayName = "XP"),
	Coin UMETA(DisplayName = "Coin"),
	Item UMETA(DisplayName = "Item"),
	Health UMETA(DisplayName = "Health"),
};

UCLASS()
class TOONTANKS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnGetFromPool();
	UFUNCTION()
	void OnReturnToPool();
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickedUp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	int32 PickupTier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	EPickupType PickupType = EPickupType::None;

	int32 PickupAmount = 0;

	UPROPERTY()
	class AToonTanksGameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CollisionAndVisuals;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UPoolableComponent* PoolableComponent;

	UPROPERTY()
	class UGravitateToPlayerInRange* GravitateToPlayerInRangeComponent;
};
