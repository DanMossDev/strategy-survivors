// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "EntityStats.h"
#include "PoolableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileStats.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	TrailParticles->SetupAttachment(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Pool = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
}

void AProjectile::OnGetFromPool(UProjectileStats* projectileStats, UEntityStats* ownerStats)
{
	OwnerStats = ownerStats;
	ProjectileMovement->SetActive(true);
	ProjectileStats = projectileStats;
	float projectileSpeed = ProjectileStats->ProjectileSpeed * OwnerStats->ProjectileSpeedMultiplier;
	ProjectileMovement->MaxSpeed = projectileSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(projectileSpeed, 0, 0));
	RemainingLifetime = ProjectileStats->ProjectileLifetime * OwnerStats->ProjectileLifetimeMultiplier;
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnCollision);
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectile::ReturnToPool()
{
	if (ProjectileMesh->OnComponentHit.IsAlreadyBound(this, &AProjectile::OnCollision))
		ProjectileMesh->OnComponentHit.RemoveDynamic(this, &AProjectile::OnCollision);
	
	Pool->ReturnToPool();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Pool = FindComponentByClass<UPoolableComponent>();

	if (Pool)
	{
		//Pool->OnGetFromPool.AddDynamic(this, &AProjectile::OnGetFromPool);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemainingLifetime -= DeltaTime;

	if (RemainingLifetime <= 0)
		HandleDestruction();
}

void AProjectile::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto owner = GetOwner();

	if (!owner) return;

	if (!OtherActor || OtherActor == this || OtherActor == owner) return;

	if (ProjectileStats)	
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileStats->DamageAmount * OwnerStats->DamageMultiplier, owner->GetInstigatorController(), this, UDamageType::StaticClass());
	
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	if (HitCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
	}
	HandleDestruction();
}

void AProjectile::HandleDestruction()
{
	ReturnToPool();
}
