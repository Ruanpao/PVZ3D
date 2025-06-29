// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "Actor/PVZ3DWeapon.h"
#include "PVZ3D/CoreTypes/PVZ3DWeaponCoreTypes.h"
#include "PVZ3DWeaponComponent.generated.h"

class APVZ3DWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PVZ3D_API UPVZ3DWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPVZ3DWeaponComponent();

	void StartFire();
	void StopFire();
	
	void Reload();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly,Category="Weapons")
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	FName WeaponAttachPointName = "WeaponSocket";
	
	UPROPERTY(EditDefaultsOnly , Category = "HoldedItem")
	APVZ3DWeapon* CurrentWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly , Category = "HoldedItem")
	TSubclassOf<class APVZ3DWeapon> DefaultWeapon = nullptr;

public:	
	
	void SwitchWeapon(FItemInInventory HoldedItem);
	
	void DestroyWeapon();
	
	
};
