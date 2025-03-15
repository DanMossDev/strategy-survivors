// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "BoomerangBullets.h"
#include "EntityStats.h"
#include "HomingProjectile.h"
#include "ObjectPoolComponent.h"
#include "PoolableComponent.h"
#include "Puddle.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileStats.h"
#include "ToonTanksGameMode.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "OrbitingProjectile.h"
#include "ProjectileLeavesTrail.h"
#include "Tile.h"
#include "WaterDamage.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Pool = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));

	HomingProjectile = CreateDefaultSubobject<UHomingProjectile>(TEXT("HomingProjectileComponent"));
	OrbitingProjectile = CreateDefaultSubobject<UOrbitingProjectile>(TEXT("OrbitingProjectileComponent"));
	TrailProjectile = CreateDefaultSubobject<UProjectileLeavesTrail>(TEXT("ProjectileLeavesTrailComponent"));
	BoomerangBullets = CreateDefaultSubobject<UBoomerangBullets>(TEXT("BoomerangBulletsComponent"));

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AProjectile::OnGetFromPool(UProjectileStats* projectileStats, UEntityStats* ownerStats, bool ShotAlternator)
{
	OwnerStats = ownerStats;
	ProjectileMovement->HomingTargetComponent = nullptr;
	ProjectileMovement->SetActive(true);
	ProjectileStats = projectileStats;
	float projectileSpeed = ProjectileStats->GetProjectileSpeed() * OwnerStats->GetProjectileSpeedMultiplier();
	ProjectileMovement->MaxSpeed = projectileSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(FVector(projectileSpeed, 0, 0));
	RemainingLifetime = ProjectileStats->GetProjectileLifetime() * OwnerStats->GetProjectileLifetimeMultiplier();
	Damage = ProjectileStats->GetDamageAmount() * OwnerStats->GetDamageMultiplier();
	ExplosionDamage = ProjectileStats->GetExplosionDamageAmount() * OwnerStats->GetExplosionDamageMultiplier();
	ExplosionSize = ProjectileStats->GetExplosionSize() * OwnerStats->GetExplosionSizeMultiplier();
	Penetrations = ProjectileStats->GetProjectilePenetrations();
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnCollision);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);


	
	if (ProjectileStats->IsHoming)
	{
		HomingProjectile->SetComponentTickEnabled(true);
		HomingProjectile->Init();
	}
	else
		HomingProjectile->SetComponentTickEnabled(false);
	
	if (ProjectileStats->IsOrbiting)
	{
		OrbitingProjectile->SetComponentTickEnabled(true);
		OrbitingProjectile->Init(ShotAlternator);
	}
	else
		OrbitingProjectile->SetComponentTickEnabled(false);
	
	if (ProjectileStats->HasTrail)
	{
		TrailProjectile->SetComponentTickEnabled(true);
		TrailProjectile->Init(ProjectileStats->Element, ProjectileCollision->GetCollisionShape());
	}
	else
		TrailProjectile->SetComponentTickEnabled(false);
	
	if (ProjectileStats->IsBoomerang)
	{
		BoomerangBullets->SetComponentTickEnabled(true);
		BoomerangBullets->Init();
	}
	else
		BoomerangBullets->SetComponentTickEnabled(false);
	
	OnSpawn();
}

void AProjectile::ReturnToPool()
{
	Pool->ReturnToPool();
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Pool = FindComponentByClass<UPoolableComponent>();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RemainingLifetime -= DeltaTime;

	if (RotateBullet)
	{
		FRotator targetRotation = FRotator(0, ProjectileMovement->Velocity.Rotation().Yaw, 0);
		SetActorRotation(targetRotation);
	}
	
	if (RemainingLifetime <= 0)
		HandleDestruction();
}

void AProjectile::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HandleDestruction();
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto owner = GetOwner();

	if (!owner) return;

	if (!OtherActor || OtherActor == this || OtherActor == owner || owner == OtherActor->GetOwner()) return;
	
	UGameplayStatics::ApplyDamage(OtherActor, Damage, owner->GetInstigatorController(), this, ProjectileStats->DamageType);
	OtherActor->AddActorWorldOffset(GetActorForwardVector() * ProjectileStats->GetKnockbackAmount() * OwnerStats->GetKnockbackMultiplier());
	
	//UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	if (!ProjectileStats->GetInfinitePenetrations())
	{
		Penetrations--;
		if (Penetrations < 0)
			HandleDestruction();
	}
}

void AProjectile::Explode()
{
	if (ProjectileStats->Element == EElementalType::Oil || ProjectileStats->Element == EElementalType::Water)
		HandleTilePuddleSpawning();
	
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(ExplosionSize);
	FVector actorPos = GetActorLocation();
	AActor* owner = GetOwner();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(owner);

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
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			if (actor && actor != owner)
			{
				UGameplayStatics::ApplyDamage(actor, ExplosionDamage, GetOwner()->GetInstigatorController(), this, ProjectileStats->DamageType);
			}
		}
	}
}

void AProjectile::HandleTilePuddleSpawning()
{
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(ExplosionSize / 2.0f);
	FVector actorPos = GetActorLocation();

	FCollisionQueryParams QueryParams;

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			ATile* tile = Cast<ATile>(actor);
			if (actor && tile)
			{
				tile->SetElement(ProjectileStats->Element);
			}
		}
	}
}

void AProjectile::HandleDestruction()
{
	if (ProjectileCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &AProjectile::OnBeginOverlap))
		ProjectileCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectile::OnBeginOverlap);
	if (ProjectileCollision->OnComponentHit.IsAlreadyBound(this, &AProjectile::OnCollision))
		ProjectileCollision->OnComponentHit.RemoveDynamic(this, &AProjectile::OnCollision);
	
	UNiagaraComponent* ns = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitVFX, GetActorLocation(), GetActorRotation());
	ns->SetFloatParameter(FName("Scale"), ExplosionSize / ExplosionRatio);
	if (ExplosionSize > 0.0f)
		Explode();
	ReturnToPool();
}

EElementalType AProjectile::GetElementalType() const
{
	return ProjectileStats->Element;
}
