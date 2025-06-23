// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void DestroyWeapon();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	TSubclassOf<APVZ3DWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	FName WeaponAttachPointName = "WeaponSocket";
	
public:	
	UPROPERTY()
	APVZ3DWeapon* CurrentWeapon =nullptr;

	void SpawnWeapon();
		
};
