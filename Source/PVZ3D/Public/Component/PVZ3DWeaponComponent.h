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

DECLARE_MULTICAST_DELEGATE_SixParams(FOnInteraction , bool , bool , bool , bool ,bool , FItemInInventory);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnConsumed , int32 , bool , bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FReloading, FText);

class APVZ3DWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PVZ3D_API UPVZ3DWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnInteraction ButtonInteraction;

	FOnConsumed OnConsumed;

	FReloading Reloading;
	
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

	UPROPERTY(EditDefaultsOnly , Category = "HoldedITtem")
	FItemInInventory CurrentHoldedItem = {0 , "0000" , 0};
	
	UPROPERTY(EditDefaultsOnly, Category = "TowerState")
	FTowerState CarriedTowerState; // 携带的塔状态

	UPROPERTY(EditDefaultsOnly, Category = "Clips")
	FText CurrentWeaponClips = FText::FromString("0");


public:	
	
	void SwitchWeapon(FItemInInventory HoldedItem);
	
	void DestroyWeapon();
	
	void BindSwitchWeapon(UPVZ3DInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	void Interact(bool IsFullTower,bool IsIntheMidLine, bool IsFullLevel , bool IsNearTower, APVZ3DTower* Tower);

	void OnReload();
};
