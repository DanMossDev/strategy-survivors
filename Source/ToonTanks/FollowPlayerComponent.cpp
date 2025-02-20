// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowPlayerComponent.h"

#include "Tank.h"
#include "BaseEntity.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFollowPlayerComponent::UFollowPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFollowPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	Entity = Cast<ABaseEntity>(GetOwner());
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}


// Called every frame
void UFollowPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CanMove) return;

	float distance = (PlayerTank->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
	if (distance <= ClosestDistance * ClosestDistance) return;
	
	Entity->RotateRoot(PlayerTank->GetActorLocation());
	Entity->AddActorLocalOffset(FVector(1.0f * BaseMovementSpeed * DeltaTime, 0.0f, 0.0f));
}

void UFollowPlayerComponent::SetCanMove(bool canMove)
{
	CanMove = canMove;
}
