// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

enum class EElementalType : uint8;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	EElementalType GetElementalType() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HandleDestruction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileCollision();

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Explode();
	void HandleTilePuddleSpawning();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	class UNiagaraSystem* HitVFX;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ExplosionRatio = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool RotateBullet = true;

	float RemainingLifetime = 0.0f;
	float ExplosionSize = 0.0f;
	float Damage = 0.0f;
	float ExplosionDamage = 0.0f;
	int32 Penetrations = 0;

	class UProjectileStats* ProjectileStats;
	class UEntityStats* OwnerStats;

	class UPoolableComponent* Pool;

	class AToonTanksGameMode* GameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnGetFromPool(UProjectileStats* projectileStats, UEntityStats* ownerStats, bool ShotAlternator);
	UFUNCTION()
	void ReturnToPool();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawn();
};
