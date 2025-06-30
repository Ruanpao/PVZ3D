// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DWeapon.h"
#include "DrawDebugHelpers.h"
#include "PVZ3DCherry.generated.h"

class APVZ3DCherryBomb;

UCLASS()
class PVZ3D_API APVZ3DCherry : public APVZ3DWeapon
{
	GENERATED_BODY()

public:
	APVZ3DCherry();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeBombPool();
	APVZ3DCherryBomb*GetBombFromPool();

	void CalculateProjectilePath();
	void ClearProjectilePath();
	void SimulateProjectilePath(FVector StartLocation, FVector LaunchVelocity, TArray<FVector>& OutPath);
	
private:
	virtual void MakeShot() override;

	//炸弹蓝图引用
	UPROPERTY(EditDefaultsOnly,Category ="Bomb")
	TSubclassOf<APVZ3DCherryBomb> BombClass;

	//炸弹容量
	UPROPERTY(EditDefaultsOnly,Category="Bomb")
	int32 BombPoolSize =5;

	//炸弹实例的数组
	UPROPERTY(EditDefaultsOnly,Category="Bomb")
	TArray<APVZ3DCherryBomb*>BombPool;

	FTimerHandle ShotTimerHandle;
	
	UPROPERTY(EditAnywhere,Category ="Weapon")
	float ThrowForce = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TimeBetweenShots = 3.0f;

	UPROPERTY(VisibleAnywhere)
	bool bIsCharging = false;		//是否准备发射

	//最大模拟时间
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float MaxSimulationTime = 3.0f;

	//模拟时间步长
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float SimulationTimeStep = 0.05f;

	//投掷物速度
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileSpeed = 2000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileGravity = 980.0f; // 重力加速度

	//储存轨迹点坐标
	TArray<FVector> PathPoints;
};
