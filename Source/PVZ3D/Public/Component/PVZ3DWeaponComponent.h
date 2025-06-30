// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "Actor/PVZ3DWeapon.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "HoldedItem")
	FName CurrentWeaponID = "0000"; // 当前武器ID
    
	UPROPERTY(EditDefaultsOnly, Category = "TowerState")
	FTowerState CarriedTowerState; // 携带的塔状态



public:	
	
	void SwitchWeapon(FItemInInventory HoldedItem);
	
	void DestroyWeapon();
	
<<<<<<< Updated upstream
	
=======
	void BindSwitchWeapon(UPVZ3DInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	void Interact(bool IsFullTower,bool IsIntheMidLine, bool IsFullLevel , bool IsNearTower, APVZ3DTower* Tower);

	void OnReload();
>>>>>>> Stashed changes
};
