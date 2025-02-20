// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* TrailParticles;

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileCollision();

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	class UProjectileStats* ProjectileStats;

	class UPoolableComponent* Pool;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnGetFromPool(UProjectileStats* Stats);
	UFUNCTION()
	void ReturnToPool();
};
