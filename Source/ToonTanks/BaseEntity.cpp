// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEntity.h"

#include "HealthComponent.h"
#include "ToonTanksGameMode.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseEntity::ABaseEntity()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	SetRootComponent(CapsuleComponent);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	SetupWeapons();
}

void ABaseEntity::SetupWeapons()
{
	for (UActorComponent* comp : GetComponents())
	{
		UWeapon* weapon = Cast<UWeapon>(comp);
		if (weapon)
			Weapons.Add(weapon);
	}
}

void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEntity::OnDeath()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	if (DeathCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
	}
}

void ABaseEntity::RotateRoot(FVector TargetLocation)
{
	FVector direction = TargetLocation - GetActorLocation();
	
	FRotator targetRotation = FRotator(0, direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotationSpeed);
	SetActorRotation(actualRotation);
}