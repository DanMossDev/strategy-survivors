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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	USceneComponent* ProjectileSpawnPoint;

	class AToonTanksGameMode* GameMode;
	
	UPROPERTY(EditAnywhere, Category = "Entity")
	float RotationSpeed = 10.0f;

	void FireProjectile();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Entity")
	UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Entity")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Entity")
	TSubclassOf<UCameraShakeBase> DeathCameraShake;

	UPROPERTY(EditAnywhere, Instanced, Category = "Entity")
	class UProjectileStats* ProjectileStats;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void RotateRoot(FVector TargetLocation);
};
