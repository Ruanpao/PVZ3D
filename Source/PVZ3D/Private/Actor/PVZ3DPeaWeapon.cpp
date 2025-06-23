// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DPeaWeapon.h"
#include"Engine/World.h"
#include "Actor/PVZ3DBullet.h"

void APVZ3DPeaWeapon::BeginPlay()
{
	Super::BeginPlay();
	InitializeProjectilePool();
}

void APVZ3DPeaWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (APVZ3DBullet* Projectile : ProjectilePool)
	{
		if (Projectile)
		{
			Projectile->Destroy();
		}
	}
	ProjectilePool.Empty();
	RecentProjectiles.Empty();
	
	Super::EndPlay(EndPlayReason);
}

void APVZ3DPeaWeapon::InitializeProjectilePool()
{
	if (!GetWorld() || !ProjectileClass) return;
	
	ProjectilePool.Empty();
	
	for (int32 i = 0; i < ProjectilePoolSize; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		APVZ3DBullet* Projectile = GetWorld()->SpawnActor<APVZ3DBullet>(
			ProjectileClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (Projectile)
		{
			Projectile->DeactivateProjectile();
			ProjectilePool.Add(Projectile);
		}
	}
}

APVZ3DBullet* APVZ3DPeaWeapon::GetProjectileFromPool()
{
	for (APVZ3DBullet* Projectile : ProjectilePool)
	{
		if (Projectile && !Projectile->IsActive())
		{
			return Projectile;
		}
	}
	
	
	if (GetWorld() && ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		APVZ3DBullet* NewProjectile = GetWorld()->SpawnActor<APVZ3DBullet>(
			ProjectileClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (NewProjectile)
		{
			NewProjectile->DeactivateProjectile();
			ProjectilePool.Add(NewProjectile);
			return NewProjectile;
		}
	}
	
	return nullptr;
}

void APVZ3DPeaWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &APVZ3DPeaWeapon::MakeShot, TimeBetweenShots, true,0.3);
}

void APVZ3DPeaWeapon::StopFire()
{

	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void APVZ3DPeaWeapon::MakeShot()
{
	if (!GetWorld()||IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}
	
	FVector ShootDirection = (TraceEnd - TraceStart).GetSafeNormal();
	
	APVZ3DBullet* Projectile = GetProjectileFromPool();
	
	if (!Projectile)
	{
		DecreaseAmmo();
		return;
	}
	
	Projectile->ActivateProjectile(
		GetMuzzleWorldLocation(),
		ShootDirection.Rotation(),
		ShootDirection
	);
	
	Projectile->IgnoreActor(GetOwner());
	
	for (APVZ3DBullet* Recent : RecentProjectiles)
	{
		if (Recent && Recent != Projectile && Recent->IsActive())
		{
			Projectile->IgnoreOtherProjectiles(Recent);
			Recent->IgnoreOtherProjectiles(Projectile);
		}
	}
	
	RecentProjectiles.Add(Projectile);
	
	if (RecentProjectiles.Num() > MaxIgnoreProjectiles)
	{
		RecentProjectiles.RemoveAt(0);
	}

	DecreaseAmmo();
}

bool APVZ3DPeaWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection =FMath::VRandCone(ViewRotation.Vector(),HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}