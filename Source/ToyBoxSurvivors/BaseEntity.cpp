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
	
	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectComponent>(TEXT("StatusEffectComponent"));
}

void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	SetupWeapons();
	HealthComponent->Init(EntityStats);

	GetComponents<UStaticMeshComponent>(Meshes);
	GetComponents<USkeletalMeshComponent>(SkeletalMeshes);

	MeshZeroPos = BaseMesh->GetRelativeLocation();
	MeshZeroRot = BaseMesh->GetRelativeRotation();
	MeshZeroScale = BaseMesh->GetRelativeScale3D();
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

	if (OverlayLerpRatio < 1.0f)
		UpdateOverlayColor(DeltaTime);
	if (DamageScaleLerpRatio < 1.0f)
		UpdateDamageScale(DeltaTime);
}

void ABaseEntity::OnDeath()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}

void ABaseEntity::RotateRoot(const FVector& TargetLocation)
{
	if (EntityStats->GetRotationSpeed() == 0.0f)
		return;
	
	FVector direction = TargetLocation - GetActorLocation();
	
	FRotator targetRotation = FRotator(0, direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), EntityStats->GetRotationSpeed());
	SetActorRotation(actualRotation);
}

void ABaseEntity::RotateRootAwayFrom(const FVector& TargetLocation)
{
	FVector direction = GetActorLocation() - TargetLocation;
	
	FRotator targetRotation = FRotator(0, direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), EntityStats->GetRotationSpeed());
	SetActorRotation(actualRotation);
}

void ABaseEntity::SetBaseMeshLocalTransform(const FVector& position, const FRotator& rotation)
{
	BaseMesh->SetRelativeLocation(position);
	BaseMesh->SetRelativeRotation(rotation);
}

void ABaseEntity::ApplyBounceToBaseMesh(float movementSpeed)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (movementSpeed == 0)
	{
		Time = 0.0f;
		FVector newPosition = FMath::Lerp(BaseMesh->GetRelativeLocation(), MeshZeroPos, DeltaTime * 8.0f);
		FRotator newRotation = FMath::Lerp(BaseMesh->GetRelativeRotation(), MeshZeroRot, DeltaTime * 8.0f);
		SetBaseMeshLocalTransform(newPosition, newRotation);
		return;
	}
	
	float movespeedRatio = movementSpeed / 250.0f;
	
	Time += DeltaTime * BounceSpeedMultiplier * movespeedRatio;
	
	FRotator newRotation = FRotator(0, 0, FMath::Sin(Time - BounceLandOffset) * BounceRollAngle);
	
	FVector newPosition = FVector(0, 0, BounceAmplitude * FMath::Sin(Time * 2) + BounceAmplitude);
	
	newPosition += FVector(0, BounceAmplitude, 0) * FMath::Cos(Time - (PI / 2) - BounceLandOffset);
	
	newPosition *= FMath::Min(Time, 1.0f);
	newPosition += MeshZeroPos;
	
	SetBaseMeshLocalTransform(newPosition, newRotation);
}

void ABaseEntity::ChargeSpell(float CastSpeed)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	Time += DeltaTime * BounceSpeedMultiplier * CastSpeed;
	
	FRotator newRotation = FRotator(0);
	FVector newPosition = FVector(0, 0, BounceAmplitude * FMath::Sin(Time * 2) + BounceAmplitude);
	
	newPosition *= FMath::Min(Time, 1.0f);
	newPosition += MeshZeroPos;
	
	SetBaseMeshLocalTransform(newPosition, newRotation);
}

void ABaseEntity::ChargeWindup(float CompletedRatio, const FRotator& TargetRotation)
{
	float sinAmount = FMath::Sin(PI * CompletedRatio);

	FVector newPosition = FMath::Lerp(MeshZeroPos, WindupLocationOffset, sinAmount);
	FRotator newRotation = FMath::Lerp(CompletedRatio >= 0.5f ? TargetRotation : MeshZeroRot, WindupRotationOffset, sinAmount);

	SetBaseMeshLocalTransform(newPosition, newRotation);
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

void ABaseEntity::TakeFireDamage(bool IsOiled)
{
	int32 amount = EntityStats->GetBurnDamagePerTick();
	if (IsOiled)
		amount *= 2;
	HealthComponent->TakeDamageManual(amount);
}

void ABaseEntity::UpdateOverlayColor(float DeltaTime)
{
	OverlayLerpRatio += DeltaTime * OverlayLerpRate;
	if (OverlayLerpRatio > 1.0f)
		OverlayLerpRatio = 1.0f;
	CurrentOverlayColor = FMath::Lerp(LastTargetOverlayColor, FColor::Black, OverlayLerpRatio);

	for (auto mesh : Meshes)
	{
		mesh->SetCustomPrimitiveDataVector4(0, CurrentOverlayColor);
	}

	for (auto skeletal : SkeletalMeshes)
	{
		skeletal->SetCustomPrimitiveDataVector4(0, CurrentOverlayColor);
	}
}

void ABaseEntity::SetOverlayColor(FLinearColor Color, bool ForceRefresh)
{
	if (Color == LastTargetOverlayColor)
	{
		OverlayLerpRatio = 0.0f;
		return;
	}
	if (OverlayLerpRatio < 0.5f && !ForceRefresh)
		return;
	
	CurrentOverlayColor = LastTargetOverlayColor = Color;
	OverlayLerpRatio = 0.0f;
}

void ABaseEntity::UpdateDamageScale(float DeltaTime)
{
	DamageScaleLerpRatio += DeltaTime * HitReactScaleSpeed;

	if (DamageScaleLerpRatio > 1.0f)
		DamageScaleLerpRatio = 1.0f;

	if (DamageScaleLerpRatio < 0.5f)
	{
		BaseMesh->SetRelativeScale3D(FMath::Lerp(MeshZeroScale, TargetScale, DamageScaleLerpRatio));
	}
	else
	{
		BaseMesh->SetRelativeScale3D(FMath::Lerp(TargetScale, MeshZeroScale, DamageScaleLerpRatio));
	}
}

void ABaseEntity::BeginHitReact()
{
	DamageScaleLerpRatio = 0.0f;
	TargetScale = MeshZeroScale * HitReactScaleMultiplier;
}
