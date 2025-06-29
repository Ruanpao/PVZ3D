// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DItemDamage.h"


APVZ3DItemDamage::APVZ3DItemDamage()
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
    
    // 投射物运动组件
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->ProjectileGravityScale = 1.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;
    ProjectileMovement->SetActive(false);
    BombMesh->SetUseCCD(true);
    ProjectileMovement->bForceSubStepping = true;
    
    // 绑定碰撞事件
    BombMesh->OnComponentHit.AddDynamic(this, &APVZ3DItemDamage::OnHit);
}

void APVZ3DItemDamage::BeginPlay()
{
    Super::BeginPlay();
    InitializeBombPool();
}

void APVZ3DItemDamage::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bIsCharging)
    {
        CalculateProjectilePath();
    }
}

void APVZ3DItemDamage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理对象池
    for (APVZ3DItemDamage* Bomb : BombPool)
    {
        if (Bomb)
        {
            Bomb->Destroy();
        }
    }
    BombPool.Empty();
    Super::EndPlay(EndPlayReason);
}

void APVZ3DItemDamage::InitializeBombPool()
{
    if (!GetWorld()) return;

    BombPool.Empty();

    for (int32 i = 0; i < BombPoolSize; i++)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        APVZ3DItemDamage* Bomb = GetWorld()->SpawnActor<APVZ3DItemDamage>(
            GetClass(),
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            SpawnParams);

        if (Bomb)
        {
            Bomb->DeactivateBomb();
            BombPool.Add(Bomb);
        }
    }
}

APVZ3DItemDamage* APVZ3DItemDamage::GetBombFromPool()
{
    for (APVZ3DItemDamage* Bomb : BombPool)
    {
        if (Bomb && !Bomb->bActive)
        {
            return Bomb;
        }
    }

    // 对象池不足时创建新实例
    if (GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        APVZ3DItemDamage* NewBomb = GetWorld()->SpawnActor<APVZ3DItemDamage>(
            GetClass(),
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            SpawnParams);

        if (NewBomb)
        {
            NewBomb->DeactivateBomb();
            BombPool.Add(NewBomb);
            return NewBomb;
        }
    }
    return nullptr;
}

void APVZ3DItemDamage::StartFire()
{
    bIsCharging = true;
}

void APVZ3DItemDamage::StopFire()
{
    if (bIsCharging)
    {
        bIsCharging = false;
        MakeShot();
    }
    ClearProjectilePath();
}

void APVZ3DItemDamage::MakeShot()
{
    if (!GetWorld()) return;

    // 获取射击方向
    FVector CameraLocation;
    FRotator CameraRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    FVector ShootDirection = CameraRotation.Vector();

    // 从对象池获取炸弹
    APVZ3DItemDamage* Bomb = GetBombFromPool();
    if (!Bomb) return;

    // 激活并发射炸弹
    Bomb->ActivateBomb(GetActorLocation(), ShootDirection);
}

void APVZ3DItemDamage::CalculateProjectilePath()
{
    // 获取射击方向
    FVector CameraLocation;
    FRotator CameraRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    FVector LaunchDirection = CameraRotation.Vector();
    FVector StartLocation = GetActorLocation();
    
    // 使用当前方向计算初始速度
    FVector LaunchVelocity = LaunchDirection * ProjectileSpeed;
    
    // 清除旧路径
    ClearProjectilePath();
    
    // 模拟轨迹
    TArray<FVector> PathPointsLocal;
    SimulateProjectilePath(StartLocation, LaunchVelocity, PathPointsLocal);
    
    // 绘制预览路径
    for (int32 i = 0; i < PathPointsLocal.Num() - 1; i++)
    {
        DrawDebugLine(
            GetWorld(),
            PathPointsLocal[i],
            PathPointsLocal[i + 1],
            FColor::Green,
            false, -1, 0,
            3.0f
        );
    }
    
    if (PathPointsLocal.Num() > 0)
    {
        DrawDebugSphere(
            GetWorld(),
            PathPointsLocal.Last(),
            30,
            12,
            FColor::Blue,
            false,
            0.1f
        );
    }
}

void APVZ3DItemDamage::SimulateProjectilePath(FVector StartLocation, FVector LaunchVelocity, TArray<FVector>& OutPath)
{
    OutPath.Empty();
    
    float Time = 0.0f;
    FVector CurrentPosition = StartLocation;
    OutPath.Add(CurrentPosition);
    
    while (Time < MaxSimulationTime)
    {
        Time += SimulationTimeStep;
        
        // 计算新位置 (s = ut + 0.5at^2)
        FVector NewPosition = StartLocation +
            LaunchVelocity * Time +
            FVector(0, 0, -0.5f * ProjectileGravity * Time * Time);
        
        // 检查碰撞
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);
        
        if (GetWorld()->LineTraceSingleByChannel(
            HitResult,
            CurrentPosition,
            NewPosition,
            ECC_Visibility,
            CollisionParams))
        {
            OutPath.Add(HitResult.Location);
            break;
        }
        
        OutPath.Add(NewPosition);
        CurrentPosition = NewPosition;
    }
}

void APVZ3DItemDamage::ClearProjectilePath()
{
    if (GetWorld())
    {
        FlushPersistentDebugLines(GetWorld());
    }
}

void APVZ3DItemDamage::ActivateBomb(FVector Location, FVector Direction)
{
    bActive = true;
    SetActorLocation(Location);
    SetActorRotation(Direction.Rotation());
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    
    ProjectileMovement->SetActive(true);
    ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovement->InitialSpeed);
    
    // 设置生命周期
    GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &APVZ3DItemDamage::DeactivateBomb, 5.0f, false);
}

void APVZ3DItemDamage::DeactivateBomb()
{
    bActive = false;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    ProjectileMovement->SetActive(false);
    ProjectileMovement->Velocity = FVector::ZeroVector;
    GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}

void APVZ3DItemDamage::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bActive) return;
    if (!OtherActor || OtherActor == GetOwner()) return;
    
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
        GetOwner()
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
