// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DCherryBomb.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/PVZ3DTower.h"


APVZ3DCherryBomb::APVZ3DCherryBomb()
{
	PrimaryActorTick.bCanEverTick = true;

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>("BombMesh");
	RootComponent = BombMesh;
    
	// 碰撞设置
	BombMesh->SetCollisionProfileName("Projectile");
	BombMesh->SetGenerateOverlapEvents(true);
	BombMesh->SetNotifyRigidBodyCollision(true);
    
	// 初始状态隐藏
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
    
	// 投射物运动组件（添加重力实现抛物线）
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f; // 启用重力实现抛物线
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->SetActive(false);
	BombMesh->SetUseCCD(true);
	ProjectileMovement->bForceSubStepping = true;
    
	// 绑定碰撞事件
	BombMesh->OnComponentHit.AddDynamic(this, &APVZ3DCherryBomb::OnHit);

}

void APVZ3DCherryBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void APVZ3DCherryBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APVZ3DCherryBomb::ActivateBomb(FVector Location, FVector Direction)
{
    bActive = true;
    
    SetActorLocation(Location);
	SetActorRotation(Direction.Rotation());
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    ProjectileMovement->SetActive(true);
    
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovement->InitialSpeed);
	
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DTower::StaticClass(), Towers);
	for (AActor* Tower : Towers)
	{
		BombMesh->IgnoreActorWhenMoving(Tower, true); 
	}
    
    GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &APVZ3DCherryBomb::DeactivateBomb, 5.0f, false);
}

void APVZ3DCherryBomb::DeactivateBomb()
{
    bActive = false;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    ProjectileMovement->SetActive(false);
    ProjectileMovement->Velocity = FVector::ZeroVector;
    GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}

void APVZ3DCherryBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision Detected with: %s"), *OtherActor->GetName());
	
    if (!bActive) return;

	if (!OtherActor|| OtherActor == GetInstigator() || OtherActor->IsA(APVZ3DTower::StaticClass())) return;
	
    // 应用范围伤害
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);
    
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

	if (ExplosionNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionNiagaraSystem,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0f), // 缩放系数
			true,          // 自动销毁
			true,          // 自动激活
			ENCPoolMethod::AutoRelease // 自动回收
		);
	}
    
    // 调试绘制爆炸范围
    // #if ENABLE_DRAW_DEBUG
    // DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 24, FColor::Red, false, 2.0f);
    // #endif
    
    DeactivateBomb();
}
