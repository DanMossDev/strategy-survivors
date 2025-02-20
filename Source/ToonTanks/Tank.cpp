// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "RotatingTurretComponent.h"
#include "ToonTanksGameMode.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"


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
	ProcessShoot(DeltaTime);
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
	if (PlayerController && InputEnabled())
	{
		FHitResult Hit;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			TurretComponent->RotateTurret(Hit.Location);
		}
	}
}

void ATank::ProcessShoot(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;

	if (ShootHeld && TimeSinceLastShot >= ShotDelay)
	{
		//Shoot
		TimeSinceLastShot = 0.0f;
		FireProjectile();
	}
}

//Input
void ATank::Move(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
}

void ATank::Shoot(const FInputActionValue& Value)
{
	ShootHeld = Value.Get<bool>();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Started, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ATank::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Canceled, this, &ATank::Move);

	EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ATank::Shoot);
	EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Completed, this, &ATank::Shoot);
	EnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Canceled, this, &ATank::Shoot);
}

FVector2D ATank::GetMoveInput() const
{
	return MoveInput;
}


APlayerController* ATank::GetPlayerController() const
{
	return PlayerController;
}
