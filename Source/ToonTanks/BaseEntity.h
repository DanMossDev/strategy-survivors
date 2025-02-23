// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEntity.generated.h"

UCLASS()
class TOONTANKS_API ABaseEntity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEntity();

	virtual void OnDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	USceneComponent* ProjectileSpawnPoint;
	class AToonTanksGameMode* GameMode;

	UPROPERTY(EditAnywhere, Instanced, Category = "Entity")
	class UEntityStats* EntityStats;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	TArray<class UWeapon*> Weapons;
	
	UPROPERTY(EditAnywhere, Category = "Entity")
	float RotationSpeed = 180.0f;

	//Visuals
	UPROPERTY(EditAnywhere, Category = "Entity")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditAnywhere, Category = "Entity")
	USoundBase* DeathSound;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void RotateRoot(FVector TargetLocation);

	UFUNCTION()
	void SetupWeapons();
};
