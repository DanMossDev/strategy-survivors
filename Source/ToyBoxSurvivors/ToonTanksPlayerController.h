// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ToonTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool bPlayerEnabled);

	bool IsUsingGamepad = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnInputChangedToController();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);
};
