// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "RotatingTurretComponent.h"
#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Cam"));
	Camera->SetupAttachment(SpringArmComponent);

	TurretComponent = CreateDefaultSubobject<URotatingTurretComponent>(TEXT("RotatingTurret"));
	TurretComponent->SetupAttachment(BaseMesh);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AToonTanksPlayerController>(GetController());
	TimeSinceLastShot = 1.0f;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessMovement(DeltaTime);
	ProcessTurretRotation(DeltaTime);
}

void ATank::OnDeath()
{
	Super::OnDeath();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
	if (GameMode)
		GameMode->GameOver();
}


void ATank::ProcessMovement(float DeltaTime)
{
	FVector Movement = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	Movement.X = MoveInput.X;
	Rotation.Yaw = MoveInput.X < 0 ? -MoveInput.Y : MoveInput.Y;
	Movement *= MovementSpeed;
	Rotation *= RotationSpeed;

	AddActorLocalOffset(Movement * DeltaTime, true);
	AddActorLocalRotation(Rotation * DeltaTime);
}

void ATank::ProcessTurretRotation(float DeltaTime)
{
	// TODO - renable if I decide to turn manual aim back on
	// if (PlayerController && InputEnabled())
	// {
	// 	FHitResult Hit;
	// 	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
	// 	if (Hit.bBlockingHit)
	// 	{
	// 		TurretComponent->RotateTurret(Hit.Location);
	// 	}
	// }

	float AttackRange = 1000.0f;
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(AttackRange);
	FVector actorPos = GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Optional: Ignore specific actors

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_Pawn,
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

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Started, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Canceled, this, &ATank::Move);
}

FVector2D ATank::GetMoveInput() const
{
	return MoveInput;
}


APlayerController* ATank::GetPlayerController() const
{
	return PlayerController;
}
