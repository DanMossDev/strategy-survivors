// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptedSpawn.h"

#include "ObjectPoolComponent.h"
#include "ToonTanksGameMode.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AScriptedSpawn::AScriptedSpawn()
{
	CollisionShape = CreateDefaultSubobject<USphereComponent>("CollisionShape");
	CollisionShape->SetCollisionProfileName(TEXT("PlayerTrigger"));
	SetRootComponent(CollisionShape);
}


void AScriptedSpawn::BeginPlay()
{
	Super::BeginPlay();

	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &AScriptedSpawn::OnBeginOverlap);

	GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
}

void AScriptedSpawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (CollisionShape->OnComponentBeginOverlap.IsAlreadyBound(this, &AScriptedSpawn::OnBeginOverlap))
		CollisionShape->OnComponentBeginOverlap.RemoveDynamic(this, &AScriptedSpawn::OnBeginOverlap);
}


void AScriptedSpawn::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CollisionShape->OnComponentBeginOverlap.IsAlreadyBound(this, &AScriptedSpawn::OnBeginOverlap))
		CollisionShape->OnComponentBeginOverlap.RemoveDynamic(this, &AScriptedSpawn::OnBeginOverlap);
	Spawn();
}

void AScriptedSpawn::Spawn()
{
	GameMode->GetObjectPool()->GetFromPool(ActorToSpawn, GetActorLocation(), GetActorRotation());
}
