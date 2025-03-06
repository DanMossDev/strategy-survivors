// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API URotateComponent : public UActorComponent
{
	GENERATED_BODY()
	URotateComponent();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation Component")
	FRotator Rotation;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
