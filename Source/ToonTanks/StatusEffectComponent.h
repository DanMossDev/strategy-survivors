// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Make an enum to select a status effect type
	//Make a map of status effect type to how long it is active for
	//Loop it per some longer tick time (0.2?) and apply effects as needed
	//Put getter methods on EntityStats and use those instead of the pure properties
	//Apply multipliers to entity stats in those getters based on active effects

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
