// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "Actor/PVZ3DWeapon.h"
#include "Actor/PVZ3DItem.h"
#include "PVZ3D/CoreTypes/PVZ3DWeaponCoreTypes.h"
#include "PVZ3D/CoreTypes/PVZ3DTowerCoreTypes.h"
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

	FName GetCurrentWeaponID() const;
	FTowerState GetCarriedTowerState() const;
    
	// 设置当前武器携带的塔状态
	void SetCarriedTowerState(const FTowerState& NewState);

	UFUNCTION(BlueprintCallable,Category="Weapon")
	int GetCurrentLevel() const { return CurrentLevel; }
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetMaxLevel() const {return MaxLevel;}
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly,Category="Weapons")
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly,Category="Weapons")
	int CurrentLevel;

	UPROPERTY(EditDefaultsOnly,Category="Weapons")
	int MaxLevel;
	
	UPROPERTY(EditDefaultsOnly , Category = "HoldedItem")
	APVZ3DWeapon* CurrentWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly , Category = "HoldedItem")
	APVZ3DItem* CurrentItem = nullptr;
    
	UPROPERTY(EditDefaultsOnly, Category = "TowerState")
	FTowerState CarriedTowerState; // 携带的塔状态



public:	
	
	void SwitchWeapon(FItemInInventory HoldedItem);
	
	void DestroyWeapon();
	void BindSwitchWeapon(UPVZ3DInventoryComponent* InventoryComponent);
	
	
};
