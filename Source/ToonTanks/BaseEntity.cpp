// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEntity.h"

#include "EntityStats.h"
#include "HealthComponent.h"
#include "StatusEffectComponent.h"
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
	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectComponent>(TEXT("StatusEffectComponent"));
}

void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	SetupWeapons();
	HealthComponent->Init(EntityStats);
	StatusEffectComponent->Init(this);
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

	HandleKnockback(DeltaTime);
}

void ABaseEntity::OnDeath()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}

void ABaseEntity::RotateRoot(FVector TargetLocation)
{
	FVector direction = TargetLocation - GetActorLocation();
	
	FRotator targetRotation = FRotator(0, direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), EntityStats->GetRotationSpeed());
	SetActorRotation(actualRotation);
}

void ABaseEntity::HandleKnockback(float DeltaTime)
{
	if (KnockbackAmount.SquaredLength() < 1.0f)
		return;

	AddActorWorldOffset(KnockbackAmount * DeltaTime);
	KnockbackAmount *= 0.9f;
}

void ABaseEntity::SetKnockbackAmount(FVector amount, float stunTime)
{
	if (stunTime > 0)
		StatusEffectComponent->AddStatusEffect(EStatusEffect::Stunned, stunTime);
	
	KnockbackAmount = amount;
}
