// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksPlayerController.h"
#include "GameFramework/InputDeviceSubsystem.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}

// void AToonTanksPlayerController::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	UInputDeviceSubsystem* Subsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
//
// 	Subsystem->OnInputHardwareDeviceChanged.AddDynamic(this, &AToonTanksPlayerController::HandleDeviceChanged);
// }

bool AToonTanksPlayerController::InputKey(const FInputKeyParams& Params)
{
	bool bResult = Super::InputKey(Params);

	if (Params.Key.IsValid())
	{
		bool gamepad = Params.IsGamepad();
		
		if (!IsUsingGamepad && gamepad)
			OnInputChangedToController();
		
		IsUsingGamepad = gamepad;
		bShowMouseCursor = IsUsingGamepad;
	}

	return bResult;
}
