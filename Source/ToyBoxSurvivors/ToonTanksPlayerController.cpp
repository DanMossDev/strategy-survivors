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

void AToonTanksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UInputDeviceSubsystem* Subsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();

	Subsystem->OnInputHardwareDeviceChanged.AddDynamic(this, &AToonTanksPlayerController::HandleDeviceChanged);
}

void AToonTanksPlayerController::HandleDeviceChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	UInputDeviceSubsystem* Subsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();

	auto device = Subsystem->GetInputDeviceHardwareIdentifier(DeviceId);

	IsUsingGamepad = device.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad;

	bShowMouseCursor = IsUsingGamepad;

	if (IsUsingGamepad)
		OnInputChangedToController();
}
