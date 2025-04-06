// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScriptedSpawn.generated.h"

UCLASS()
class TOONTANKS_API AScriptedSpawn : public AActor
{
	GENERATED_BODY()
	
public:
	AScriptedSpawn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Spawn();

	UPROPERTY(VisibleAnywhere, Category = "ScriptedSpawn")
	class USphereComponent* CollisionShape;

	UPROPERTY(EditAnywhere, Category = "ScriptedSpawn")
	TSubclassOf<AActor> ActorToSpawn;

	class AToonTanksGameMode* GameMode;
};
