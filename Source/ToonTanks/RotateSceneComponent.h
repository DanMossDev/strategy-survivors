// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RotateSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API URotateSceneComponent : public USceneComponent
{
	GENERATED_BODY()
	URotateSceneComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation Component")
	FRotator Rotation;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
