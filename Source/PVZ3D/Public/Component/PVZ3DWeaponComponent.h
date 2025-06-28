// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	void SwitchWeapon();
	void Reload();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly,Category="Weapons")
	UDataTable*WeaponDataTable;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

	UPROPERTY()
	APVZ3DWeapon* CurrentWeapon =nullptr;

	UPROPERTY()
	TArray<APVZ3DWeapon*>WeaponInstances;
	
	int32 CurrentWeaponIndex=0;
public:	
	
	void SpawnWeapon(const FWeapon&WeaponData);
	void LoadWeapons();
	void DestroyWeapon();
	
};
