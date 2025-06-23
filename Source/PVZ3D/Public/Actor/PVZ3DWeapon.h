// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DWeapon.generated.h"

class USkeletalMeshComponent;

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
};


UCLASS()
class PVZ3D_API APVZ3DWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APVZ3DWeapon();

	virtual void StartFire();
	virtual void StopFire();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	FName MuzzleSocketName = "MuzzSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	float TraceMaxDistance = 1500.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeBetweenShots = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	FAmmoData DefaultAmmo{15,10,false};

	void MakeShot();
	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd)const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, FVector& TraceEnd) const;
	void MakeDamage(const FHitResult& HitResult);

	void DecreaseAmmo();
	bool IsAmmoEmpty() const;	//全弹夹为空
	bool IsClipEmpty() const;	//当前弹夹为空
	void ChangeClip();
	void LogAmmo();

private:
	FTimerHandle ShotTimerHandle;

	FAmmoData CurrentAmmo;
};
