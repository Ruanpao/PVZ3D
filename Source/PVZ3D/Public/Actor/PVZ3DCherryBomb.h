// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/PVZ3DItemDamage.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "PVZ3DCherryBomb.generated.h"

UCLASS()
class PVZ3D_API APVZ3DCherryBomb : public APVZ3DItemDamage
{
	GENERATED_BODY()
	
public:	
	APVZ3DCherryBomb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ActivateBomb(FVector Location,FVector Direction);
	void DeactivateBomb();
	bool IsActive() const {return bActive;}

	//爆炸伤害
	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount = 50.0f;	
    
	UPROPERTY(EditAnywhere, Category = "Damage")
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ExplosionNiagaraSystem;

private:
	//网格体
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BombMesh;

	//投掷物运动组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	//是否激活
	bool bActive = false;

	//炸弹生命周期
	FTimerHandle LifeSpanTimer;
};
