// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Projectile.h"
#include "Components/BoxComponent.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	SetRootComponent(Collision);

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile Mesh");
	TileMesh->SetupAttachment(Collision);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

	FVector footAdjustedPosition = GetActorLocation();
	footAdjustedPosition.Z = TileMesh->Bounds.BoxExtent.Z;
	SetActorLocation(footAdjustedPosition);

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
}

void ATile::SetElement(EElementalType Element)
{
	RemainingLifetime = 5;
	CurrentElement = Element;
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

bool ATile::GetOverlappingEnemies(TArray<FOverlapResult>& OverlappingActors)
{
	FVector actorPos = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Collision->GetCollisionShape(),
		QueryParams
	);

	return bHit;
}

void ATile::CheckOverlappingProjectiles()
{
	TArray<FOverlapResult> OverlappingActors;
	FVector actorPos = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel7,
		Collision->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		for (FOverlapResult& OverlappingActor : OverlappingActors)
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
