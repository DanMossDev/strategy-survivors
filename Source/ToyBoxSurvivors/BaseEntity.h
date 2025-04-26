// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEntity.generated.h"

UCLASS(Abstract)
class TOONTANKS_API ABaseEntity : public APawn
{
	GENERATED_BODY()

public:
	ABaseEntity();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	USceneComponent* ProjectileSpawnPoint;
	
	class AToonTanksGameMode* GameMode;

	UPROPERTY(EditAnywhere, Instanced, Category = "Entity")
	class UEntityStats* EntityStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity")
	class UStatusEffectComponent* StatusEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	bool IsPriorityTarget = false;

	void SetOverlayColor(FLinearColor Color);
	void BeginHitReact();
	
protected:
	virtual void BeginPlay() override;

	void HandleKnockback(float DeltaTime);

	virtual float GetCurrentMovementSpeed() const {return 0.0f;}
	virtual void UpdateOverlayColor(float DeltaTime);
	virtual void UpdateDamageScale(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	TArray<class UWeapon*> Weapons;
	TArray<UStaticMeshComponent*> Meshes;
	TArray<USkeletalMeshComponent*> SkeletalMeshes;

	FVector KnockbackAmount;

	//Visuals
	UPROPERTY(EditAnywhere, Category = "Entity")
	UParticleSystem* DeathParticles;
	UPROPERTY(EditAnywhere, Category = "Entity")
	USoundBase* DeathSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity")
	float OverlayLerpRate = 4.0f;
	
	UPROPERTY(EditAnywhere, Category = "Entity")
	float BounceRollAngle = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Entity")
	float BounceLandOffset = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Entity")
	float BounceSpeedMultiplier = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Entity")
	float BounceAmplitude = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Entity")
	FVector WindupLocationOffset;
	UPROPERTY(EditAnywhere, Category = "Entity")
	FRotator WindupRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Entity")
	float HitReactScaleMultiplier = 0.9f;
	UPROPERTY(EditAnywhere, Category = "Entity")
	float HitReactScaleSpeed = 4.0f;
	
	FVector MeshZeroPos;
	FRotator MeshZeroRot;
	FVector MeshZeroScale;

	FLinearColor LastTargetOverlayColor = FColor::Black;
	FLinearColor CurrentOverlayColor = FColor::Black;
	float OverlayLerpRatio = 1.0f;

	FVector TargetScale;
	float DamageScaleLerpRatio = 1.0f;

	float Time = 0.0f;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	void RotateRoot(const FVector& TargetLocation);
	void RotateRootAwayFrom(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable)
	void SetBaseMeshLocalTransform(const FVector& position, const FRotator& rotation);

	UFUNCTION()
	void SetupWeapons();

	virtual void OnDeath();
	void SetKnockbackAmount(FVector amount, float stunTime = 0.0f);

	void ResetAnimTime() {Time = 0.0f;}
	
	void ApplyBounceToBaseMesh(float movementSpeed);
	void ChargeSpell(float CastSpeed);
	void ChargeWindup(float CompletedRatio, const FRotator& TargetRotation);

	UFUNCTION(BlueprintImplementableEvent, Category = "Entity")
	void OnUpdateStatusEffectUI();
	
	void TakeFireDamage(bool IsOiled);
};
