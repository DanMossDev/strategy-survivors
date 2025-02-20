// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "PoolableComponent.h"
#include "Components/SphereComponent.h"
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

void AProjectile::OnGetFromPool(UProjectileStats* Stats)
{
	ProjectileMovement->SetActive(true);
	ProjectileStats = Stats;
	ProjectileMovement->MaxSpeed = ProjectileStats->ProjectileSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(ProjectileStats->ProjectileSpeed, 0, 0));
	
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
}

void AProjectile::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto owner = GetOwner();

	if (!owner) return;

	if (!OtherActor || OtherActor == this || OtherActor == owner) return;

	if (ProjectileStats)	
		UGameplayStatics::ApplyDamage(OtherActor, ProjectileStats->DamageAmount, owner->GetInstigatorController(), this, UDamageType::StaticClass());
	
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	if (HitCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
	}
	ReturnToPool();
}