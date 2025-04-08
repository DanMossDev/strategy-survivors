// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnlockableData.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();

	TMap<class UUnlockableData*, int32> GetInventory() const {return Inventory;}
	TArray<UUnlockableData*> GetUpgradeables() const;
	TArray<class UWeaponInfo*> GetMergedWeapons() const {return MergedWeapons;}
	TArray<UWeaponInfo*> GetEvolveables() const;
	int32 AddToInventory(UUnlockableData* ItemAdded, int32 ItemLevel);

	void WeaponMerged(UWeaponInfo* ConsumedWeapon);
	void EvolveWeapon(UWeaponInfo* EvolvedWeapon);

	bool CanFindNewWeapons() const;
	bool CanFindNewStats() const;
	bool IsUnlockableMaxed(const UUnlockableData* Stat) const;

protected:
	virtual void BeginPlay() override;

	TMap<UUnlockableData*, int32> Inventory;

	TMap<UWeaponInfo*, int32> Weapons;
	TArray<UWeaponInfo*> MergedWeapons;
	TArray<UWeaponInfo*> EvolvedWeapons;
	TMap<class UStatBoost*, int32> StatBoosts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 MaxWeaponCount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 MaxStatBoostCount = 5;
};
