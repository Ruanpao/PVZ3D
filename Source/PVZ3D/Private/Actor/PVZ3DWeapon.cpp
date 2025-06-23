// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DWeapon.h"
#include"Components/SkeletalMeshComponent.h"
#include"Engine/World.h"
#include"DrawDebugHelpers.h"
#include "Character/PVZ3DPlayer.h"
#include"GameFramework/Character.h"
#include"GameFramework/Controller.h"
#include"Engine/DamageEvents.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon,All,All)

APVZ3DWeapon::APVZ3DWeapon()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void APVZ3DWeapon::StartFire()
{
}

void APVZ3DWeapon::StopFire()
{
}

void APVZ3DWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

	CurrentAmmo = DefaultAmmo;
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

void APVZ3DWeapon::MakeShot()
{
}

bool APVZ3DWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection =ViewRotation.Vector();
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