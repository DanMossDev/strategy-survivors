// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EntityStats.h"
#include "EventDispatcher.h"
#include "HealthComponent.h"
#include "Inventory.h"
#include "PhysicalDamage.h"
#include "PlayerHealthComponent.h"
#include "RotatingTurretComponent.h"
#include "StatBoost.h"
#include "StatusEffectComponent.h"
#include "SurvivorGameInstance.h"
#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Cam"));
	Camera->SetupAttachment(SpringArmComponent);

	TurretComponent = CreateDefaultSubobject<URotatingTurretComponent>(TEXT("RotatingTurret"));
	TurretComponent->SetupAttachment(BaseMesh);

	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("HealthComponent"));

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AToonTanksPlayerController>(GetController());
	TimeSinceLastShot = 1.0f;

	FVector footAdjustedPosition = GetActorLocation();
	footAdjustedPosition.Z = CapsuleComponent->GetScaledCapsuleHalfHeight() + 2;
	SetActorLocation(footAdjustedPosition);

	for (auto weapon : StarterWeapons)
	{
		GameMode->SelectItem(weapon);
		if (BeginEvolved)
			GameMode->SelectEvolveable(weapon);
	}
	for (auto stat : StarterStats)
		GameMode->SelectStat(stat);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessMovement(DeltaTime);
	ProcessTurretRotation(DeltaTime);

	CheckForEnemyCollisions();

	ApplyBounceToBaseMesh(GetCurrentMovementSpeed());

	if (CachedMovement > 100.0f)
	{
		UEventDispatcher::AddToStat(EStatsType::DistanceTravelled, CachedMovement / 100.0f);
		CachedMovement = 0.0f;
	}
}

float ATank::GetCurrentMovementSpeed() const
{
	return FMath::Min(MoveInput.Length(), 1.0) * EntityStats->GetMovementSpeed();
}

void ATank::OnDeath()
{
	Super::OnDeath();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
	if (GameMode)
		GameMode->GameOver(false);
}

void ATank::ProcessMovement(float DeltaTime)
{
	if (MoveInput == FVector2D::ZeroVector)
		return;

	if (StatusEffectComponent->HasStatusEffect(EStatusEffect::Stunned) || StatusEffectComponent->HasStatusEffect(EStatusEffect::Frozen))
		return;
	
	FVector Direction = FVector(MoveInput.X, MoveInput.Y, 0.0f).GetSafeNormal();

	FRotator targetRotation = FRotator(0, Direction.Rotation().Yaw, 0);
	FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, EntityStats->GetRotationSpeed());
	SetActorRotation(actualRotation);

	CachedMovement += EntityStats->GetMovementSpeed() * DeltaTime;

	AddActorWorldOffset(Direction * EntityStats->GetMovementSpeed() * DeltaTime, true);
}

void ATank::ProcessTurretRotation(float DeltaTime)
{
	if (ManualAim && InputEnabled())
	{
		if (!PlayerController->IsUsingGamepad)
		{
			FHitResult Hit;
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			if (Hit.bBlockingHit)
			{
				TurretComponent->RotateTurret(Hit.Location);
			}
		}
		else
		{
			if (AimInput.SquaredLength() < 0.1f)
				return;
			
			FVector aimLocation = GetActorLocation();
			aimLocation.X += AimInput.Y * -1000;
			aimLocation.Y += AimInput.X * 1000;
			TurretComponent->RotateTurret(aimLocation);
		}
		return;
	}
	
	float AttackRange = EntityStats->GetAttackRangeMultiplier() * 1000;
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(AttackRange);
	FVector actorPos = GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		bool anyEntityFound = false;
		FVector Target = FVector::ZeroVector;
		float previousDistance = 9999999;
		for (FOverlapResult overlap : OverlappingActors)
		{
			ABaseEntity* entity = Cast<ABaseEntity>(overlap.GetActor());
			if (entity)
			{
				anyEntityFound = true;
				if (entity->IsPriorityTarget)
				{
					Target = entity->GetActorLocation();
					break;
				}
				FVector difference = actorPos - entity->GetActorLocation();
				float distance = difference.SquaredLength();
				if (distance < previousDistance)
				{
					Target = entity->GetActorLocation();
					previousDistance = distance;
				}
			}
		}
		if (anyEntityFound)
			TurretComponent->RotateTurret(Target);
	}
}

//Input
void ATank::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
}

void ATank::Aim(const FInputActionValue& Value)
{
	AimInput = Value.Get<FVector2D>();
}

void ATank::ToggleManualAimInput(const FInputActionValue& Value)
{
	ToggleManualAim();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Started, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Canceled, this, &ATank::Move);

	EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Triggered, this, &ATank::Aim);

	EnhancedInputComponent->BindAction(ToggleManualAimInputAction, ETriggerEvent::Triggered, this, &ATank::ToggleManualAim);
}

FVector2D ATank::GetMoveInput() const
{
	return MoveInput;
}

APlayerController* ATank::GetPlayerController() const
{
	return PlayerController;
}

void ATank::AddStatBoost(UStatBoost* StatBoost)
{
	*EntityStats += *StatBoost->Effect;
	if (CollectedStatBoosts.Contains(StatBoost->Name))
	{
		CollectedStatBoosts[StatBoost->Name]++;
	}
	else
	{
		CollectedStatBoosts.Add(StatBoost->Name, 1);
	}
}

void ATank::Heal(int32 amount)
{
	HealthComponent->Heal(amount);
}

void ATank::ShowDamageTaken()
{
	GetPlayerController()->ClientStartCameraShake(HitCameraShake);
}

void ATank::CheckForEnemyCollisions()
{
	if (HealthComponent->GetIsInvincible())
		return;
	
	TArray<FOverlapResult> OverlappingActors;
	FVector actorPos = GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		CapsuleComponent->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		for (FOverlapResult overlap : OverlappingActors)
		{
			if (AEnemy* enemy = Cast<AEnemy>(overlap.GetActor()))
			{
				UGameplayStatics::ApplyDamage(this, enemy->EntityStats->GetContactDamageAmount(), enemy->GetInstigatorController(), enemy, UPhysicalDamage::StaticClass());
				enemy->SetKnockbackAmount(enemy->GetActorForwardVector() * -enemy->EntityStats->GetKnockbackAmount(), 1.0f);

				SetKnockbackAmount(enemy->GetActorForwardVector() * enemy->EntityStats->GetKnockbackAmount(), 0.25f);
				return;
			}
		}
	}
}
