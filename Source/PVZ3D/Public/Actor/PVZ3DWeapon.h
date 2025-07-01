// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DBullet.h"
#include "Kismet/GameplayStatics.h"
#include "AI/PVZ3DTower.h"
#include "PVZ3DWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_MULTICAST_DELEGATE(FOnReload);
DECLARE_MULTICAST_DELEGATE_OneParam(FDecrease ,int32);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon",meta=(EditCondition="!Infinite"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	bool Infinite;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Weapon")
	float ReloadTime = 2.0f ;
};

UCLASS()
class PVZ3D_API APVZ3DWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	FOnReload OnReload;

	FDecrease Decrease;
	
	APVZ3DWeapon();

	virtual void StartFire();
	virtual void StopFire();

	virtual void StartReload();
	virtual bool CanReload() const;

	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	void SetAttribute(float Damage,float Distance,float ReloadTime,float Clips);

protected:
	virtual void BeginPlay() override;

	virtual void Reload();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	FName MuzzleSocketName = "MuzzSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	float TraceMaxDistance = 1500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	FAmmoData DefaultAmmo{150,1000,false};
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Animation")
	UAnimMontage*ReloadAnimMontage;
	
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd)const;
	
	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd) const;
	void MakeDamage(const FHitResult& HitResult);

	void DecreaseAmmo();
	bool IsAmmoEmpty() const;	//全弹夹为空
	bool IsClipEmpty() const;	//当前弹夹为空
	void ChangeClip();
	void LogAmmo();

	bool bIsReloading = false;

	float BulletDamage = 0.0f;
private:
	FTimerHandle ShotTimerHandle;

	FAmmoData CurrentAmmo;

	FTimerHandle ReloadTimerHandle;
};
