// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "EntityStats.h"
#include "ObjectPoolComponent.h"
#include "PoolableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileStats.h"
#include "ToonTanksGameMode.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	Pool = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
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
	Damage = ProjectileStats->DamageAmount * OwnerStats->DamageMultiplier;
	ExplosionDamage = ProjectileStats->ExplosionDamageAmount * OwnerStats->ExplosionDamageMultiplier;
	ExplosionSize = ProjectileStats->ExplosionSize * OwnerStats->ExplosionSizeMultiplier;
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

	ProjectileCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnCollision);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	OnSpawn();
}

void AProjectile::ReturnToPool()
{
	if (ProjectileCollision->OnComponentBeginOverlap.IsAlreadyBound(this, &AProjectile::OnBeginOverlap))
		ProjectileCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectile::OnBeginOverlap);
	if (ProjectileCollision->OnComponentHit.IsAlreadyBound(this, &AProjectile::OnCollision))
		ProjectileCollision->OnComponentHit.RemoveDynamic(this, &AProjectile::OnCollision);
	
	Pool->ReturnToPool();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Pool = FindComponentByClass<UPoolableComponent>();
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
	HandleDestruction();
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto owner = GetOwner();

	if (!owner) return;

	if (!OtherActor || OtherActor == this || OtherActor == owner) return;
	
	UGameplayStatics::ApplyDamage(OtherActor, Damage, owner->GetInstigatorController(), this, UDamageType::StaticClass());
	
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	if (HitCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
	}
	HandleDestruction();
}

void AProjectile::Explode()
{
	//Show explosion effect
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionSize, 10, FColor::Red, false, 3.0f);
	//If elemental, spawn appropriate decal
	
	TArray<FOverlapResult> OverlappingActors;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(ExplosionSize);
	FVector actorPos = GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

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
		auto owner = GetOwner();
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			if (actor && actor != owner)
			{
				UGameplayStatics::ApplyDamage(actor, ExplosionDamage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
			}
		}
	}
}


void AProjectile::HandleDestruction()
{
	UNiagaraComponent* ns = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HitVFX, GetActorLocation(), GetActorRotation());
	ns->SetFloatParameter(FName("Scale"), ExplosionSize / ExplosionRatio);
	if (ExplosionSize > 0.0f)
		Explode();
	ReturnToPool();
}
