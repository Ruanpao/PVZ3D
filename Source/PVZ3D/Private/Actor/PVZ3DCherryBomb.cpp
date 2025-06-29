// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DCherryBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"



APVZ3DCherryBomb::APVZ3DCherryBomb() : APVZ3DItemDamage()
{
	DamageAmount = 500.0f;          // 樱桃炸弹的伤害值
	ExplosionRadius = 600.0f;       // 樱桃炸弹的爆炸范围
	ProjectileSpeed = 1800.0f;      // 樱桃炸弹的投掷速度
	ProjectileGravity = 980.0f;     // 樱桃炸弹的重力影响
	TimeBetweenShots = 5.0f;        // 樱桃炸弹的使用冷却时间

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CherryMesh(TEXT("/Game/PlantsVsZombies3D/Props/Meshes/CherryBomb"));
	if (CherryMesh.Succeeded() && BombMesh)
	{
		BombMesh->SetStaticMesh(CherryMesh.Object);
	}
    
	// 材质
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CherryMaterial(TEXT("/Game/PlantsVsZombies3D/Props/Materials/M_CherryBomb"));
	if (CherryMaterial.Succeeded() && BombMesh)
	{
		BombMesh->SetMaterial(0, CherryMaterial.Object);
	}
    
	// 爆炸特效
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CherryExplosion(TEXT("/Game/PlantsVsZombies3D/VFX/NS_CherryExplosion"));
	if (CherryExplosion.Succeeded())
	{
		ExplosionNiagaraSystem = CherryExplosion.Object;
	}
}

void APVZ3DCherryBomb::StartFire()
{
	// 调用父类方法显示抛物线轨迹预览
	Super::StartFire();
}

void APVZ3DCherryBomb::StopFire()
{
	// 调用父类方法投掷炸弹并清除轨迹预览
	Super::StopFire();
}

void APVZ3DCherryBomb::MakeShot()
{
	// 调用父类方法创建炸弹实例并投掷
	Super::MakeShot();
}

void APVZ3DCherryBomb::CalculateProjectilePath()
{
	// 调用父类方法计算抛物线轨迹
	Super::CalculateProjectilePath();
}

void APVZ3DCherryBomb::ClearProjectilePath()
{
	// 调用父类方法清除轨迹预览
	Super::ClearProjectilePath();
}

void APVZ3DCherryBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 调用父类方法处理碰撞和爆炸
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void APVZ3DCherryBomb::ActivateBomb(FVector Location, FVector Direction, AActor* NewWeaponOwner, AController* NewWeaponInstigator)
{
	// 调用父类方法激活炸弹
	Super::ActivateBomb(Location, Direction, NewWeaponOwner, NewWeaponInstigator);
}

void APVZ3DCherryBomb::DeactivateBomb()
{
	// 调用父类方法停用炸弹
	Super::DeactivateBomb();
}