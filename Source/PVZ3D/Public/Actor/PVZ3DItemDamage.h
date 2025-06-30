// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DItem.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PVZ3DItemDamage.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DItemDamage : public APVZ3DItem
{
    GENERATED_BODY()

public:    
    APVZ3DItemDamage();

protected:
    bool bIsCharging = true;
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 抛物线预览
    virtual void CalculateProjectilePath();
    virtual void ClearProjectilePath();
    virtual void SimulateProjectilePath(FVector StartLocation, FVector LaunchVelocity, TArray<FVector>& OutPath);

    // 射击控制
    virtual void StartUse() override;
    virtual void StopUse() override;
    virtual void MakeShot();

    // 炸弹激活/禁用
    virtual void ActivateBomb(FVector Location, FVector Direction, AActor* NewWeaponOwner, AController* NewWeaponInstigator);
    virtual void DeactivateBomb();
public:    

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    // 武器配置
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamageAmount = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Damage")
    float ExplosionRadius = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    float ThrowForce = 800.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float TimeBetweenShots = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UNiagaraSystem* ExplosionNiagaraSystem;

    // 抛物线模拟参数
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float MaxSimulationTime = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float SimulationTimeStep = 0.05f;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float ProjectileSpeed = 2000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float ProjectileGravity = 980.0f;

    bool bActive = false;
private:
    // 状态控制
    TArray<FVector> PathPoints;
    FTimerHandle LifeSpanTimer;

    AActor* WeaponOwner = nullptr;
    AController* WeaponInstigator = nullptr;
};