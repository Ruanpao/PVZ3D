// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DCherry.h"
#include "Actor/PVZ3DCherryBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"

APVZ3DCherry::APVZ3DCherry()
{
	DamageAmount = 300.f;
	TimeBetweenShots =3.0f;

	MaxSimulationTime = 3.0f;
	SimulationTimeStep = 0.05f;
	ProjectileGravity = 980.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void APVZ3DCherry::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeBombPool();
}

void APVZ3DCherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsCharging)
	{
		CalculateProjectilePath();
	}
}

void APVZ3DCherry::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//对象池清理
	for(APVZ3DCherryBomb*Bomb :BombPool)
	{
		if(Bomb)
		{
			Bomb->Destroy();
		}
	}
	BombPool.Empty();

	Super::EndPlay(EndPlayReason);
}

void APVZ3DCherry::InitializeBombPool()
{
	if(!GetWorld()||!BombClass) return;

	BombPool.Empty();

	for(int32 i=0;i<BombPoolSize;i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APVZ3DCherryBomb*Bomb =GetWorld()->SpawnActor<APVZ3DCherryBomb>(
			BombClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams);
		if(Bomb)
		{
			//Bomb->DeactivateBomb();
			BombPool.Add(Bomb);
		}
	}
}

APVZ3DCherryBomb* APVZ3DCherry::GetBombFromPool()
{
	for(APVZ3DCherryBomb*Bomb :BombPool)
	{
		//if(Bomb&&!Bomb->IsActive())
		//{
		//		return Bomb;
		//}
	}

	//扩容
	if(GetWorld()&&BombClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		APVZ3DCherryBomb*NewBomb =GetWorld()->SpawnActor<APVZ3DCherryBomb>(
			BombClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams);

		if(NewBomb)
		{
			//NewBomb->DeactivateBomb();
			BombPool.Add(NewBomb);
			return NewBomb;
		}
	}
	return nullptr;
}

void APVZ3DCherry::StartFire()
{
	bIsCharging = true;
	
}

void APVZ3DCherry::StopFire()
{
	if (bIsCharging)
	{
		bIsCharging = false;
		MakeShot();
	}
	ClearProjectilePath();
}

void APVZ3DCherry::MakeShot()
{
	if(!GetWorld()||IsAmmoEmpty()) return;

	FVector TraceStart , TraceEnd ;
	if(!GetTraceData(TraceStart,TraceEnd)) return;

	FVector ShootDirection =(TraceEnd - TraceStart).GetSafeNormal();

	APVZ3DCherryBomb * Bomb =GetBombFromPool();
	if(!Bomb) return;

	//Bomb->ActivateBomb(GetMuzzleWorldLocation(),ShootDirection);

	DecreaseAmmo();
	
}

void APVZ3DCherry::CalculateProjectilePath()
{
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FVector LaunchDirection = (TraceEnd - TraceStart).GetSafeNormal();
	FVector StartLocation = GetMuzzleWorldLocation();
    
	// 使用当前方向计算初始速度
	FVector LaunchVelocity = LaunchDirection * ProjectileSpeed;
	
	// 清除旧路径
	ClearProjectilePath();
    
	// 模拟轨迹
	SimulateProjectilePath(StartLocation, LaunchVelocity, PathPoints);
    
	// 绘制预览路径
	for (int32 i = 0; i < PathPoints.Num() - 1; i++)
	{
		DrawDebugLine(
			GetWorld(),
			PathPoints[i],
			PathPoints[i + 1],
			FColor::Green,
			false, -1, 0,
			3.0f // 线宽
		);
	}
	
	if(PathPoints.Num() > 0)
	{
		DrawDebugSphere(
			GetWorld(),
			PathPoints.Last(),
			30,
			12,
			FColor::Blue,
			false,
			0.1f
		);
	}
}

// 模拟抛体轨迹
void APVZ3DCherry::SimulateProjectilePath(FVector StartLocation, FVector LaunchVelocity, TArray<FVector>& OutPath)
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

// 清除预览路径
void APVZ3DCherry::ClearProjectilePath()
{
	if (GetWorld())
	{
		FlushPersistentDebugLines(GetWorld());
	}
	PathPoints.Empty();
}