// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DWeapon.h"
#include"Components/SkeletalMeshComponent.h"
#include"Engine/World.h"
#include"DrawDebugHelpers.h"
#include "Character/PVZ3DPlayer.h"
#include"GameFramework/Character.h"
#include"GameFramework/Controller.h"
#include"Engine/DamageEvents.h"
#include"Camera/CameraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon,All,All)

APVZ3DWeapon::APVZ3DWeapon()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void APVZ3DWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

	CurrentAmmo = DefaultAmmo;
}

void APVZ3DWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &APVZ3DWeapon::MakeShot, TimeBetweenShots, true,0.3);
	
}

void APVZ3DWeapon::StopFire()
{

	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void APVZ3DWeapon::MakeShot()
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

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Green, false, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

APlayerController* APVZ3DWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return  Player->GetController <APlayerController>();
}

bool APVZ3DWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector APVZ3DWeapon::GetMuzzleWorldLocation()const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool APVZ3DWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	// 获取Camera组件
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!Camera) {
		UE_LOG(LogWeapon, Error, TEXT("未找到Camera组件！"));
		return false;
	}

	// 射线起点 = Camera位置
	TraceStart = Camera->GetComponentLocation();
    
	// 射线方向 = Camera旋转方向 + 随机散布
	FRotator CameraRotation = Camera->GetComponentRotation();
	const float HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(CameraRotation.Vector(), HalfRad);

	// 射线终点 = 起点 + 方向 * 最大射程
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void APVZ3DWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

}

void APVZ3DWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return;

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void APVZ3DWeapon::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;
	LogAmmo();

	if(!IsAmmoEmpty()&&IsClipEmpty())
	{
		ChangeClip();
	}
}

bool APVZ3DWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite&&CurrentAmmo.Clips ==0&&IsClipEmpty();
}

bool APVZ3DWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets==0;
}

void APVZ3DWeapon::ChangeClip()
{
	CurrentAmmo.Bullets =DefaultAmmo.Bullets;

	if(!CurrentAmmo.Infinite)
	{
		CurrentAmmo.Clips--;
	}
	UE_LOG(LogTemp, Display, TEXT("-------------change---------------"));
}

void APVZ3DWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo +=CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogTemp, Display, TEXT("%s"), *AmmoInfo);
}