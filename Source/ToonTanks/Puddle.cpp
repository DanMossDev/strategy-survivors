// Fill out your copyright notice in the Description page of Project Settings.


#include "Puddle.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PoolableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuddle::APuddle()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("Collision"));
	SetRootComponent(CapsuleCollision);

	Poolable = CreateDefaultSubobject<UPoolableComponent>("Poolable");
}

void APuddle::Init(EElementalType Type, float Scale)
{
	IsDying = false;
	Element = Type;
	RefreshVisuals();
	CurrentScale = 0;
	SetActorScale3D(FVector(Scale));
	TargetScale = Scale;
	RemainingLifetime = Lifetime;
}


void APuddle::BeginPlay()
{
	Super::BeginPlay();
}

void APuddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDying)
	{
		CurrentScale -= DeltaTime * TargetScale * 5.0f;
		if (CurrentScale < 0)
		{
			HandleDestroy();
			return;
		}

		SetActorScale3D(FVector(CurrentScale));
		return;
	}

	// if (CurrentScale < TargetScale)
	// {
	// 	CurrentScale += DeltaTime * TargetScale * 2.0f;
	// 	if (CurrentScale > TargetScale)
	// 		CurrentScale = TargetScale;
	//
	// 	SetActorScale3D(FVector(CurrentScale));
	// }
	FVector loc = GetActorLocation();
	if (loc.Z > 0)
	{
		loc.Z -= DeltaTime * 1000.0f;

		if (loc.Z < 0)
		{
			loc.Z = 0;
		}
		SetActorLocation(loc);
	}

	RemainingLifetime -= DeltaTime;

	if (Element == EElementalType::Fire)
	{
		TArray<FOverlapResult> OverlappingActors;
		FVector actorPos = GetActorLocation();
	
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
	
		bool bHit = GetWorld()->OverlapMultiByChannel(
			OverlappingActors,
			actorPos,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			CapsuleCollision->GetCollisionShape(),
			QueryParams
		);
	
		if (bHit)
		{
			for (auto overlapResult : OverlappingActors)
			{
				APuddle* puddle = Cast<APuddle>(overlapResult.GetActor());
		
				if (puddle)
				{
					puddle->HitByElement(Element, false);
				}
			}
		}
	}

	if (RemainingLifetime <= 0)
		IsDying = true;
}

void APuddle::HitByElement(EElementalType IncomingElement, bool ShouldRefresh)
{
	if (IsDying || IncomingElement == Element)
	{
		return;
	}

	if (( IncomingElement == EElementalType::Fire && Element == EElementalType::Oil) || (IncomingElement == EElementalType::Oil && Element == EElementalType::Fire))
	{
		if (ShouldRefresh)
			RemainingLifetime = Lifetime;
		if (Element != EElementalType::Fire)
		{
			Element = EElementalType::Fire;
			FireExplosion();
			RefreshVisuals();
		}
	}

	if (( IncomingElement == EElementalType::Ice && Element == EElementalType::Water) || (IncomingElement == EElementalType::Water && Element == EElementalType::Ice))
	{
		if (ShouldRefresh)
			RemainingLifetime = Lifetime;
		if (Element != EElementalType::Ice)
		{
			Element = EElementalType::Ice;
			//Freeze
			RefreshVisuals();
		}
	}
}

void APuddle::AddToPuddle(EElementalType IncomingElement, float Scale)
{
	if (IsDying) return;

	if (IncomingElement == Element)
	{
		RemainingLifetime = Lifetime;
		TargetScale += Scale;
		if (TargetScale > Scale * 2)
			TargetScale = Scale * 2;
		return;
	}

	if (( IncomingElement == EElementalType::Fire && Element == EElementalType::Oil) || (IncomingElement == EElementalType::Oil && Element == EElementalType::Fire))
	{
		RemainingLifetime = Lifetime;
		TargetScale += Scale;
		if (TargetScale > Scale * 2)
			TargetScale = Scale * 2;
		
		Element = EElementalType::Fire;
		
		FireExplosion();
		RefreshVisuals();
	}
}

void APuddle::FireExplosion()
{
	UNiagaraComponent* ns = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FlameExplosion, GetActorLocation(), GetActorRotation());
	ns->SetFloatParameter(FName("Scale"), GetActorScale().X);

	//Do fire explosion damage
	TArray<FOverlapResult> OverlappingActors;
	FVector actorPos = GetActorLocation();
	AActor* owner = GetOwner();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(owner);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlappingActors,
		actorPos,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		CapsuleCollision->GetCollisionShape(),
		QueryParams
	);

	if (bHit)
	{
		for (auto overlapResult : OverlappingActors)
		{
			AActor* actor = overlapResult.GetActor();
			if (actor && actor != owner)
			{
				UGameplayStatics::ApplyDamage(actor, 10, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass()); //TODO work out how to get explosion damage
			}
		}
	}
}


void APuddle::HandleDestroy()
{
	Poolable->ReturnToPool();
}
