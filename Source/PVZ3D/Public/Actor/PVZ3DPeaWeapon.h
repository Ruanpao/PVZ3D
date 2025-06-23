// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DWeapon.h"
#include "PVZ3DPeaWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DPeaWeapon : public APVZ3DWeapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<APVZ3DBullet> ProjectileClass;
	
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd)const override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FTimerHandle ShotTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile Pool")
	int32 ProjectilePoolSize = 20;
	
	TArray<APVZ3DBullet*> ProjectilePool;
	
	void InitializeProjectilePool();
	
	APVZ3DBullet* GetProjectileFromPool();
	
	TArray<APVZ3DBullet*> RecentProjectiles;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxIgnoreProjectiles = 50;
};
