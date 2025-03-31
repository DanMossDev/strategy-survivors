// Fill out your copyright notice in the Description page of Project Settings.


#include "ColumnOfFire.h"

#include "PoolableComponent.h"
#include "Kismet/GameplayStatics.h"

AColumnOfFire::AColumnOfFire()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(Mesh);
	Poolable = CreateDefaultSubobject<UPoolableComponent>(TEXT("PoolableComponent"));
}

void AColumnOfFire::BeginPlay()
{
	Super::BeginPlay();

	Poolable->OnGetFromPool.AddDynamic(this, &AColumnOfFire::OnGetFromPool);
	Poolable->OnReturnToPool.AddDynamic(this, &AColumnOfFire::OnReturnToPool);
}

void AColumnOfFire::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Poolable->OnGetFromPool.IsAlreadyBound(this, &AColumnOfFire::OnGetFromPool))
		Poolable->OnGetFromPool.RemoveDynamic(this, &AColumnOfFire::OnGetFromPool);
	if (Poolable->OnReturnToPool.IsAlreadyBound(this, &AColumnOfFire::OnReturnToPool))
		Poolable->OnReturnToPool.RemoveDynamic(this, &AColumnOfFire::OnReturnToPool);
}


void AColumnOfFire::Init(const FVector& Scale, float TelegraphTime, float AttackTime, float Damage)
{
	TargetScale = Scale;

	TelegraphDuration = TelegraphTime;
	AttackDuration = AttackTime;
	OutgoingDamage = Damage;

	Mesh->SetMaterial(0, TelegraphingMaterial);

	Location = GetActorLocation();
	Location.Z = 0;
	SetActorLocation(Location);

	TimeAlive = 0.0f;
	ShowingFire = false;
}

void AColumnOfFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAlive += DeltaTime;

	if (TimeAlive < TelegraphDuration)
	{
		SetActorScale3D(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), TargetScale, FMath::Min(TimeAlive / TelegraphDuration * 2.0f, 1.0f)));
		Location.Z = GetActorScale().Z;
		SetActorLocation(Location);
		return;
	}

	if (TimeAlive < AttackDuration + TelegraphDuration)
	{
		if (!ShowingFire)
		{
			Mesh->SetMaterial(0, FireMaterial);
			ShowingFire = true;
		}
		SetActorScale3D(FMath::Lerp(FVector(TargetScale.X, TargetScale.Y, 0),TargetScale, FMath::Min((TimeAlive - TelegraphDuration) * 4.0f, 1.0f)));
		CheckForPlayer();
		return;
	}

	const float t = (TimeAlive - AttackDuration - TelegraphDuration) * 3.0f;
	if (t > 1)
	{
		Poolable->ReturnToPool();
		return;
	}
	SetActorScale3D(FMath::Lerp(TargetScale, FVector(0.0f, 0.0f, 0.0f), t));
	Location.Z = GetActorScale().Z;
	SetActorLocation(Location);
}

void AColumnOfFire::OnGetFromPool()
{
	SetActorScale3D(FVector(0));
}

void AColumnOfFire::OnReturnToPool()
{
	SetActorScale3D(FVector(0));
}

void AColumnOfFire::CheckForPlayer()
{
	TArray<FOverlapResult> Overlaps;
	FVector actorPos = GetActorLocation();
	
	FCollisionQueryParams QueryParams;
	
	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		Mesh->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		UGameplayStatics::ApplyDamage(Overlaps[0].GetActor(), OutgoingDamage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	}
}
