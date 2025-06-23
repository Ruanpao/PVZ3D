// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DSunflowerWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include"Engine/World.h"

APVZ3DSunflowerWeapon::APVZ3DSunflowerWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentDamageMultiplier = 1.0f;
    LastFireTime = 0.0f;
}

void APVZ3DSunflowerWeapon::StartFire()
{
    Super::StartFire();
    
    if (LaserEffect && WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
    {
        LaserBeam = UNiagaraFunctionLibrary::SpawnSystemAttached(
            LaserEffect,
            WeaponMesh,
            MuzzleSocketName,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
    }
    
    GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APVZ3DSunflowerWeapon::ApplyContinuousDamage, 0.2f, true);
}

void APVZ3DSunflowerWeapon::StopFire()
{
    Super::StopFire();
    
    GetWorldTimerManager().ClearTimer(DamageTimerHandle);
    ResetDamageAccumulation();
    
    if (LaserBeam)
    {
        LaserBeam->DestroyComponent();
        LaserBeam = nullptr;
    }
}

void APVZ3DSunflowerWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateLaserBeam();
}

void APVZ3DSunflowerWeapon::UpdateLaserBeam()
{
    if (!LaserBeam) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(FName("Enemy")))
    {
        AActor* HitActor = HitResult.GetActor();
        
        UNiagaraComponent* HitEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
            HitEffect,
            HitActor->GetRootComponent(),
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget,
            true
        );
        
        if (HitEffectComponent)
        {
            FTimerHandle TimerHandle;
            FTimerDelegate TimerDelegate;
            TimerDelegate.BindLambda([HitEffectComponent]()
            {
                if (HitEffectComponent && HitEffectComponent->IsValidLowLevel())
                {
                    HitEffectComponent->DestroyComponent();
                }
            });
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
        }
    }

    AActor* NewTarget = HitResult.GetActor();
    if (NewTarget != CurrentTarget)
    {
        ResetDamageAccumulation();
        CurrentTarget = NewTarget;
    }
}

void APVZ3DSunflowerWeapon::ApplyContinuousDamage()
{
    if (!CurrentTarget || IsAmmoEmpty()) return;

    const float DamageToApply = BaseDamagePerSecond * 0.2f * CurrentDamageMultiplier;
    UGameplayStatics::ApplyDamage(
        CurrentTarget,
        DamageToApply,
        GetPlayerController(),
        this,
        UDamageType::StaticClass()
    );

    if (GetWorld()->GetTimeSeconds() - LastFireTime >= 1.0f)
    {
        DecreaseAmmo();
        LastFireTime = GetWorld()->GetTimeSeconds();
    }

    CurrentDamageMultiplier = FMath::Min(CurrentDamageMultiplier + DamageMultiplierIncrement, MaxDamageMultiplier);
}

void APVZ3DSunflowerWeapon::ResetDamageAccumulation()
{
    CurrentTarget = nullptr;
    CurrentDamageMultiplier = 1.0f;
}