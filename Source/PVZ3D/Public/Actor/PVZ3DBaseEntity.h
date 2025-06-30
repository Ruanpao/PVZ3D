// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PVZ3DBaseEntity.generated.h"
//#include "InventoryComponent.h"


class InventoryComponent;
class HealthComponent;
class WeaponComponent;

UCLASS()
class PVZ3D_API APVZ3DBaseEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APVZ3DBaseEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MoveForward(const float Val);
	virtual void MoveRight(const float Val);
	virtual void LookUp(const float Val);
	virtual void LookRight(const float Val);

	virtual void Attack();
	virtual void Interact();
	virtual void StartRun();
	virtual void StopRun();
	virtual void UpdateCurrentWeaponID();
	
	bool IsMovingForward = false;
	bool IsMovingRight = false;

	int CurrentWeaponID = 0;
	int AggroValue=0;
	int Velocity=0;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact(AActor* InstigatorActor) {};


};
