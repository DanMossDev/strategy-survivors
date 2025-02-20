// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEntity.h"

#include "HealthComponent.h"
#include "ObjectPoolComponent.h"
#include "PoolableComponent.h"
#include "Projectile.h"
#include "ProjectileStats.h"
#include "ToonTanksGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	CapsuleComponent->SetupAttachment(Root);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
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

void ABaseEntity::FireProjectile()
{
	FVector actorLocation = GetActorLocation();
	FRotator actorRotation = ProjectileSpawnPoint->GetComponentRotation();
	float angle = 360 / ProjectileStats->ProjectileCount;
	for (int i = 0; i < ProjectileStats->ProjectileCount; i++)
	{
		FVector spawnOffset = ProjectileSpawnPoint->GetComponentLocation() - actorLocation;
		FRotator rotationOffset = FRotator(0, angle * i, 0);
		spawnOffset = rotationOffset.RotateVector(spawnOffset);
		
		AProjectile* Projectile = GameMode->GetObjectPool()->GetFromPool<AProjectile>(ProjectileClass, actorLocation + spawnOffset, actorRotation + rotationOffset);
		
		if (!Projectile)
			return;
		
		Projectile->SetOwner(this);
		Projectile->OnGetFromPool(ProjectileStats);
	}
	
}
