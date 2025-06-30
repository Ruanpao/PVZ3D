// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PVZ3DBullet.generated.h"

UCLASS()
class PVZ3D_API APVZ3DBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	APVZ3DBullet();
	
protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	void ActivateProjectile(FVector Location, FRotator Rotation, FVector Direction);
	
	void DeactivateProjectile();
	
	bool IsActive() const { return bActive; }
	
	void IgnoreActor(AActor* Actor);
	
	void IgnoreOtherProjectiles(AActor* ProjectileToIgnore);
	
	float GetProjectileSpeed() const;
	
	void SetProjectileSpeed(float NewSpeed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DamageAmount = 10.0f;

	bool bActive = false;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle LifeSpanTimer;
};
