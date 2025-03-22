// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "ObjectPoolComponent.h"
#include "Projectile.h"
#include "StatusEffectComponent.h"
#include "TileMesh.h"
#include "ToonTanksGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	SetRootComponent(Collision);

	// TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile Mesh");
	// TileMesh->SetupAttachment(Collision);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	// FVector footAdjustedPosition = GetActorLocation();
	// footAdjustedPosition.Z = TileMesh->Bounds.BoxExtent.Z;
	// SetActorLocation(footAdjustedPosition);

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	SetActorTickEnabled(false);
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RemainingLifetime -= DeltaTime;

	if (RemainingLifetime <= 0)
	{
		SetElement(EElementalType::None);
		return;
	}
	
	CheckOverlappingProjectiles();

	if (CurrentElement == EElementalType::Fire)
		CheckOverlappingEnemies();
}

void ATile::SetElement(EElementalType Element)
{
	RemainingLifetime = 5;
	
	if (CurrentElement == EElementalType::Ice && Element == EElementalType::Water)
		Element = EElementalType::Ice;
	else if (CurrentElement == EElementalType::Fire && Element == EElementalType::Oil)
		Element = EElementalType::Fire;
	
	CurrentElement = Element;

	if (Element != EElementalType::None)
	{
		if (TileMesh == nullptr)
			TileMesh = GameMode->GetObjectPool()->GetFromPool<ATileMesh>(TileClass, GetActorLocation(), GetActorRotation());
		else
			TileMesh->RefreshVisuals();
	}
	else
	{
		if (TileMesh)
		{
			TileMesh->Poolable->ReturnToPool();
			TileMesh = nullptr;
		}
	}
	
	OnElementChanged(Element);

	SetActorTickEnabled(Element != EElementalType::None);
}

void ATile::HitByElement(EElementalType Element)
{
	if ((Element == EElementalType::Fire && CurrentElement == EElementalType::Oil) || (Element == EElementalType::Oil && CurrentElement == EElementalType::Fire))
	{
		//FireExplosion
		SetElement(EElementalType::Fire);
	}

	if ((Element == EElementalType::Ice && CurrentElement == EElementalType::Water) || (Element == EElementalType::Water && CurrentElement == EElementalType::Ice))
	{
		SetElement(EElementalType::Ice);
	}
}

void ATile::CheckOverlappingEnemies()
{
	OverlapCache.Empty();
	FVector actorPos = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapCache,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Collision->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		for (FOverlapResult& OverlappingActor : OverlapCache)
		{
			UStatusEffectComponent* seComp = OverlappingActor.GetActor()->FindComponentByClass<UStatusEffectComponent>();
			if (seComp)
			{
				seComp->AddStatusEffect(EStatusEffect::Burning, 1.0f);
				return;
			}
		}
	}
}

void ATile::CheckOverlappingProjectiles()
{
	OverlapCache.Empty();
	FVector actorPos = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapCache,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel7,
		Collision->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		for (FOverlapResult& OverlappingActor : OverlapCache)
		{
			AProjectile* projectile = Cast<AProjectile>(OverlappingActor.GetActor());
			if (projectile)
			{
				HitByElement(projectile->GetElementalType());
				return;
			}
		}
	}
}
