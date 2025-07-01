// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DCherryBomb.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"



APVZ3DCherryBomb::APVZ3DCherryBomb() 
{
	DamageAmount = 500.0f;          // 樱桃炸弹的伤害值
	ExplosionRadius = 600.0f;       // 樱桃炸弹的爆炸范围
	ProjectileSpeed = 1800.0f;      // 樱桃炸弹的投掷速度
	ProjectileGravity = 980.0f;     // 樱桃炸弹的重力影响
	TimeBetweenShots = 5.0f;        // 樱桃炸弹的使用冷却时间

	ProjectileMovement->ProjectileGravityScale = ProjectileGravity / 980.0f;

	ItemMesh->OnComponentHit.AddDynamic(this, &APVZ3DCherryBomb::OnHit);

	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 显式禁用碰撞
	
}

void APVZ3DCherryBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void APVZ3DCherryBomb::StartUse()
{
	Super::StartUse();
}

void APVZ3DCherryBomb::StopUse()
{
	Super::StopUse();
}

void APVZ3DCherryBomb::MakeShot()
{
	Super::MakeShot();
}

void APVZ3DCherryBomb::CalculateProjectilePath()
{
	Super::CalculateProjectilePath();
}

void APVZ3DCherryBomb::ClearProjectilePath()
{
	Super::ClearProjectilePath();
}

void APVZ3DCherryBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bActive) return;
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	UE_LOG(LogTemp, Warning, TEXT("Bomb hit: %s"), OtherActor ? *OtherActor->GetName() : TEXT("None"));
	// 应用范围伤害
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	if (GetOwner()) {
		IgnoredActors.Add(GetOwner()); // 添加角色到忽略列表
	}
    
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		DamageAmount,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoredActors,
		this,
		GetInstigatorController()
	);

	// 播放爆炸特效
	if (ExplosionNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionNiagaraSystem,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease
		);
	}
    
	// 调试绘制爆炸范围
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 24, FColor::Red, false, 2.0f);
    
	DeactivateBomb();
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