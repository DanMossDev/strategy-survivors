// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowPlayerPawn.generated.h"

UCLASS()
class TOONTANKS_API AFollowPlayerPawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AFollowPlayerPawn();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};