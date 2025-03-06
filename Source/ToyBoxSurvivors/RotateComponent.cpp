// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateComponent.h"

URotateComponent::URotateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetOwner()->AddActorLocalRotation(Rotation * DeltaTime);
}
