// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelStreamingManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API ULevelStreamingManager : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevelStreamingManager();
	void Initialize();

protected:
	void UpdateLevelStreaming() const;

	UPROPERTY()
	UWorld* World;
	UPROPERTY()
	class AToonTanksGameMode* GameMode;
	UPROPERTY()
	class ATank* Player;
	
	UPROPERTY(EditAnywhere, Category="LevelStreaming")
	float LoadDistance = 5000.0f;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
