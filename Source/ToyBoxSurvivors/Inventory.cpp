// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "StatBoost.h"
#include "WeaponInfo.h"

UInventory::UInventory()
{
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
}

TArray<UUnlockableData*> UInventory::GetUpgradeables() const
{
	TArray<UUnlockableData*> upgradeables = TArray<UUnlockableData*>();

	for (auto& tuple : Inventory)
	{
		if (tuple.Value < tuple.Key->MaxLevel && !MergedWeapons.Contains(tuple.Key))
			upgradeables.Add(tuple.Key);
	}

	return upgradeables;
}

TArray<UWeaponInfo*> UInventory::GetEvolveables() const
{
	TArray<UWeaponInfo*> evolveables = TArray<UWeaponInfo*>();

	for (auto& tuple : Weapons)
	{
		if (*tuple.Value < tuple.Key->MaxLevel || !tuple.Key->HasEvolution || EvolvedWeapons.Contains(tuple.Key))
			continue;

		bool canAdd = true;
		for (auto requirement : tuple.Key->EvolutionRequirements)
			if (!Inventory.Contains(requirement) || Inventory[requirement] < requirement->MaxLevel)
				canAdd = false;
		
		if (canAdd)
			evolveables.Add(tuple.Key);
	}

	return evolveables;
}

int32 UInventory::AddToInventory(UUnlockableData* ItemAdded, int32 ItemLevel)
{
	if (!Inventory.Contains(ItemAdded))
	{
		Inventory.Add(ItemAdded, ItemLevel - 1);
		if (ItemAdded->IsA(UWeaponInfo::StaticClass()))
			Weapons.Add(Cast<UWeaponInfo>(ItemAdded), &Inventory[ItemAdded]);
		else
			StatBoosts.Add(Cast<UStatBoost>(ItemAdded), &Inventory[ItemAdded]);

		return ItemLevel;
	}

	if (Inventory[ItemAdded] < ItemAdded->MaxLevel)
	{
		int32 startingLevel = Inventory[ItemAdded];
		Inventory[ItemAdded] = FMath::Min(ItemAdded->MaxLevel, Inventory[ItemAdded] + ItemLevel);
		return Inventory[ItemAdded] - startingLevel;
	}

	return 0;
}

void UInventory::WeaponMerged(UWeaponInfo* ConsumedWeapon)
{
	Weapons.Remove(ConsumedWeapon);

	MergedWeapons.Add(ConsumedWeapon);
}

void UInventory::EvolveWeapon(UWeaponInfo* EvolvedWeapon)
{
	EvolvedWeapons.Add(EvolvedWeapon);
}

bool UInventory::CanFindNewWeapons() const
{
	return Weapons.Num() < MaxWeaponCount;
}

bool UInventory::CanFindNewStats() const
{
	return StatBoosts.Num() < MaxStatBoostCount;
}

bool UInventory::IsUnlockableMaxed(const UUnlockableData* Item) const
{
	if (!Inventory.Contains(Item))
		return false;

	return Inventory[Item] >= Item->MaxLevel;
}