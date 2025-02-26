// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateSceneComponent.h"

URotateSceneComponent::URotateSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URotateSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddRelativeRotation(Rotation * DeltaTime);
}

