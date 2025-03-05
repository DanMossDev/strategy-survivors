// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockableData.h"

#include "SurvivorGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UUnlockableData::Unlock(UObject* worldContextObject)
{
	IsUnlocked = true;
	
	if (USurvivorGameInstance* instance = Cast<USurvivorGameInstance>(UGameplayStatics::GetGameInstance(worldContextObject)))
		instance->GetSaveFile()->UnlockedItems.Add(Name, IsUnlocked);
}
