// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DWeapon.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "PVZ3DSunflowerWeapon.generated.h"

UCLASS()
class PVZ3D_API APVZ3DSunflowerWeapon : public APVZ3DWeapon
{
	GENERATED_BODY()

public:
	APVZ3DSunflowerWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* LaserEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float DamageMultiplierIncrement = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float MaxDamageMultiplier = 4.0f;

private:
	UPROPERTY()
	UNiagaraComponent* LaserBeam;

	UPROPERTY()
	AActor* CurrentTarget;

	float CurrentDamageMultiplier;
	float LastFireTime;
	FTimerHandle DamageTimerHandle;
	float ContinuousFireTime;//射击时间

	void UpdateLaserBeam();
	void ApplyContinuousDamage();
	void ResetDamageAccumulation();
	void HandleOverheat();//熄火了
};