#pragma once
#include "Engine/DataTable.h"
#include "PVZ3DWeaponCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName WeaponID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponName ="None";

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category ="Weapon")
	TSubclassOf<APVZ3DWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UTexture2D* WeaponIcon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = 0))
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Combat")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = 0))
	float ReloadTime = 2.0f;
	
	// 弹药系统
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	int32 Bullets = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon",meta=(EditCondition="!Infinite"))
	int32 Clips =-1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Weapon")
	bool Infinite =false;	

	// 资源引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Assets")
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Assets")
	TSubclassOf<class APVZ3DBullet> BulletClass;
};