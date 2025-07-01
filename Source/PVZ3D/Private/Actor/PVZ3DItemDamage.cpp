// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DItemDamage.h"


APVZ3DItemDamage::APVZ3DItemDamage()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 碰撞设置
    ItemMesh->SetCollisionProfileName("NoCollision");
    ItemMesh->SetGenerateOverlapEvents(true);
    ItemMesh->SetNotifyRigidBodyCollision(true);
    ItemMesh->SetSimulatePhysics(false);
    
    
    // 投射物运动组件
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    ProjectileMovement->InitialSpeed = ProjectileSpeed;
    ProjectileMovement->MaxSpeed = ProjectileSpeed *1.5f;
    
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;
    ProjectileMovement->SetActive(false);
    ItemMesh->SetUseCCD(true);
    ProjectileMovement->bForceSubStepping = true;
    
    
}

void APVZ3DItemDamage::BeginPlay()
{
    Super::BeginPlay();
    
}

void APVZ3DItemDamage::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bIsCharging)
    {
        CalculateProjectilePath();
    }
}

void APVZ3DItemDamage::StartUse()
{
    bIsCharging = true;
    UE_LOG(LogTemp, Warning, TEXT("use"));
}

void APVZ3DItemDamage::StopUse()
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
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = WeaponOwner;
    SpawnParams.Instigator = Cast<APawn>(WeaponOwner);
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // 使用当前炸弹的类创建新实例
    APVZ3DItemDamage* Item = GetWorld()->SpawnActor<APVZ3DItemDamage>(
        GetClass(),
        GetActorLocation(),
        ShootDirection.Rotation(),
        SpawnParams
    );
    Item->bIsCharging=false;
    if (Item)
    {
        // 激活并发射炸弹
        Item->ItemMesh->SetMobility(EComponentMobility::Movable);
        Item->ActivateBomb(GetActorLocation(), ShootDirection,GetOwner(),GetInstigatorController());
    }
   
}

void APVZ3DItemDamage::CalculateProjectilePath()
{
    // 获取射击方向
    FVector CameraLocation;
    FRotator CameraRotation;

    if (!GetWorld() || !GetWorld()->GetFirstPlayerController()) return;
    
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
        if (GetOwner()) {
            CollisionParams.AddIgnoredActor(GetOwner()); // 忽略角色
        }
        
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

void APVZ3DItemDamage::ActivateBomb(FVector Location, FVector Direction, AActor* NewWeaponOwner, AController* NewWeaponInstigator)
{
    this->WeaponOwner = NewWeaponOwner;
    this->WeaponInstigator = NewWeaponInstigator;
    SetOwner(NewWeaponOwner);
    
    bActive = true;
    SetActorLocation(Location);
    SetActorRotation(Direction.Rotation());
    
    ProjectileMovement->SetActive(true);
    ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileMovement->InitialSpeed);
    
    // 设置生命周期
    GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &APVZ3DItemDamage::DeactivateBomb, 5.0f, false);
}

void APVZ3DItemDamage::DeactivateBomb()
{
    Destroy();
}