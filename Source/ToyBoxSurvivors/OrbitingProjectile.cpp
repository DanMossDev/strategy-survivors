// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitingProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

UOrbitingProjectile::UOrbitingProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitingProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
}

void UOrbitingProjectile::Init(bool ShotAlternator)
{
	Lifetime = 0.0f;
	ShotAlternated = ShotAlternator;
}

void UOrbitingProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Lifetime += DeltaTime;

	if (Lifetime > StartingTime)
		RotateProjectile(DeltaTime);
}

void UOrbitingProjectile::RotateProjectile(float DeltaTime)
{
	FRotator Rotator = FRotator(0, 360, 0) * RotationRate * DeltaTime;
	if (DecayingOrbit)
		Rotator *= 1.0f/Lifetime;
	if (AlternateDirection && ShotAlternated)
		Rotator *= -1.0f;
	ProjectileMovement->Velocity = Rotator.RotateVector(ProjectileMovement->Velocity);
	// GetOwner()->AddActorLocalRotation(FRotator(0, 360, 0) * RotationRate * DeltaTime);
}
