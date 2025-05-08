// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilitySystem.h"

#include "EnhancedInputComponent.h"
#include "PlayerAbility.h"
#include "Tank.h"

UPlayerAbilitySystem::UPlayerAbilitySystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerAbilitySystem::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ATank>(GetOwner());
	PlayerAbilities = TArray<UPlayerAbility*>();
	PlayerAbilities.SetNum(4);
}

void UPlayerAbilitySystem::InitialiseActionMappings(UEnhancedInputComponent* EnhancedInputComponent)
{
	// EnhancedInputComponent->BindAction(ExampleInputAction, ETriggerEvent::Triggered, this, &UPlayerAbilitySystem::SomeMethod);
	// EnhancedInputComponent->BindAction(ExampleInputAction, ETriggerEvent::Started, this, &UPlayerAbilitySystem::SomeMethod);
	// EnhancedInputComponent->BindAction(ExampleInputAction, ETriggerEvent::Completed, this, &UPlayerAbilitySystem::SomeMethod);
	// EnhancedInputComponent->BindAction(ExampleInputAction, ETriggerEvent::Canceled, this, &UPlayerAbilitySystem::SomeMethod);

	
	EnhancedInputComponent->BindAction(Ability1InputAction, ETriggerEvent::Completed, this, &UPlayerAbilitySystem::CastAbility1);
	EnhancedInputComponent->BindAction(Ability2InputAction, ETriggerEvent::Completed, this, &UPlayerAbilitySystem::CastAbility2);
	EnhancedInputComponent->BindAction(Ability3InputAction, ETriggerEvent::Completed, this, &UPlayerAbilitySystem::CastAbility3);
	EnhancedInputComponent->BindAction(Ability4InputAction, ETriggerEvent::Completed, this, &UPlayerAbilitySystem::CastAbility4);
}

void UPlayerAbilitySystem::RegisterAbility(UPlayerAbility* Ability, int Slot)
{
	if (Ability == nullptr)
		return;
	
	Ability->InjectOwner(this);
	PlayerAbilities[Slot] = Ability;
}

void UPlayerAbilitySystem::TickComponent(float DeltaTime,  ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GlobalCooldownRemaining > 0.0f)
	{
		GlobalCooldownRemaining -= DeltaTime;

		if (GlobalCooldownRemaining < 0.0f)
			GlobalCooldownRemaining = 0.0f;
	}

	for (auto ability : PlayerAbilities)
	{
		if (ability)
			ability->UpdateAbility(DeltaTime);
	}
}

void UPlayerAbilitySystem::SetGlobalCooldown(float Cooldown)
{
	if (GlobalCooldownRemaining < Cooldown)
		MostRecentGlobalCooldown = GlobalCooldownRemaining = Cooldown;
}

float UPlayerAbilitySystem::GetRemainingGlobalCooldown() const
{
	return GlobalCooldownRemaining / MostRecentGlobalCooldown;
}

float UPlayerAbilitySystem::GetRemainingCooldownForAbility(int32 Slot) const
{
	if (PlayerAbilities[Slot])
	{
		return PlayerAbilities[Slot]->GetCooldownRatio();
	}

	return 0.0f;
}

UTexture2D* UPlayerAbilitySystem::GetSpriteForAbility(int32 Slot) const
{
	if (PlayerAbilities[Slot])
	{
		return PlayerAbilities[Slot]->Sprite;
	}

	return nullptr;
}


bool UPlayerAbilitySystem::IsMovementLocked() const
{
	for (auto ability : PlayerAbilities)
	{
		if (!ability || !ability->IsMovementLocked())
			continue;

		return true;
	}

	return false;
}

bool UPlayerAbilitySystem::IsShootingLocked() const
{
	for (auto ability : PlayerAbilities)
	{
		if (!ability || !ability->IsShootingLocked())
			continue;

		return true;
	}

	return false;
}

bool UPlayerAbilitySystem::IsCastingLocked() const
{
	for (auto ability : PlayerAbilities)
	{
		if (!ability || !ability->IsCastingLocked())
			continue;

		return true;
	}

	return false;
}

void UPlayerAbilitySystem::CastAbility1(const FInputActionValue& Value)
{
	if (PlayerAbilities[0])
		PlayerAbilities[0]->TryCastAbility();
}

void UPlayerAbilitySystem::CastAbility2(const FInputActionValue& Value)
{
	if (PlayerAbilities[1])
		PlayerAbilities[1]->TryCastAbility();
}

void UPlayerAbilitySystem::CastAbility3(const FInputActionValue& Value)
{
	if (PlayerAbilities[2])
		PlayerAbilities[2]->TryCastAbility();
}

void UPlayerAbilitySystem::CastAbility4(const FInputActionValue& Value)
{
	if (PlayerAbilities[3])
		PlayerAbilities[3]->TryCastAbility();
}
