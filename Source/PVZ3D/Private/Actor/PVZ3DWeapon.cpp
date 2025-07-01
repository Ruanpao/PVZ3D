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
#include "Clustering/FaceNormalClustering.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All)

APVZ3DWeapon::APVZ3DWeapon()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	
}

void APVZ3DWeapon::SetAttribute(float Damage, float Distance, float ReloadTime, float Clips)
{
	DamageAmount = Damage;
	
	TraceMaxDistance = Distance;
	CurrentAmmo.ReloadTime = ReloadTime;
	if(Clips)
	{
		DefaultAmmo.Bullets = Clips;
	}
	else
	{
		DefaultAmmo.Bullets = 30;
	}
	CurrentAmmo = DefaultAmmo;
}

void APVZ3DWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	
	bIsReloading =false;
}

void APVZ3DWeapon::StartFire()
{
   UE_LOG(LogTemp, Display, TEXT("Start Fire!"));
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
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (!Camera) {
		UE_LOG(LogWeapon, Error, TEXT("未找到Camera组件！"));
		return false;
	}

	TraceStart = Camera->GetComponentLocation();
	
	FRotator CameraRotation = Camera->GetComponentRotation();
	const FVector ShootDirection = CameraRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void APVZ3DWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd) const
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DTower::StaticClass(), Towers);
	for (AActor* Tower : Towers)
	{
		CollisionParams.AddIgnoredActor(Tower);
	}
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);

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

	if(!DefaultAmmo.Infinite)
	{
		UE_LOG(LogWeapon , Warning , TEXT("123 , %d"),CurrentAmmo.Bullets);
		Decrease.Broadcast(CurrentAmmo.Bullets);
	}

	if(IsClipEmpty()&&!IsAmmoEmpty())
	{
		StartReload();
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

void APVZ3DWeapon::StartReload()
{
	if(!CanReload()) return;

	bIsReloading = true;
	StopFire();

	if(ReloadAnimMontage)
	{
		if(const auto Character = Cast<ACharacter>(GetOwner()))
		{
			if(UAnimInstance*AnimInstance=Character->GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(ReloadAnimMontage);
			}
			
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &APVZ3DWeapon::Reload,CurrentAmmo.ReloadTime,false);
}

bool APVZ3DWeapon::CanReload() const
{
	return !bIsReloading&&!CurrentAmmo.Infinite&&CurrentAmmo.Clips>0&&CurrentAmmo.Bullets<DefaultAmmo.Bullets;
}

void APVZ3DWeapon::Reload()
{
	bIsReloading = false;
	ChangeClip();

	UE_LOG(LogWeapon , Warning , TEXT("Reloaded! Current Ammo: %d Bullets, %d Clips"), CurrentAmmo.Bullets, CurrentAmmo.Clips);
	
	OnReload.Broadcast();
}
