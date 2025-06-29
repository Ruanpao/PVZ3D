#pragma once
#include "Engine/DataTable.h"
#include "Actor/PVZ3DWeapon.h"
#include "Actor/PVZ3DBullet.h"
#include "PVZ3DWeaponCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FWeaponBasicInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FName Name = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	UTexture2D* Icon = nullptr;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float Damage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float TraceMaxDistance = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	float ReloadTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FText Clips = FText::FromString("0");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	TSubclassOf<class APVZ3DWeapon> WeaponClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	TSubclassOf<class APVZ3DBullet> BulletClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	FName ItemType = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponInfo")
	bool Infinite = false;
};

