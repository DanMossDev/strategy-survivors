// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowPlayerPawn.h"

#include "Kismet/GameplayStatics.h"

AFollowPlayerPawn::AFollowPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFollowPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFollowPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());
}

