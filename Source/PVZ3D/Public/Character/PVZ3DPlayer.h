// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "GenericTeamAgentInterface.h"
#include "PVZ3DPlayer.generated.h"



class UCameraComponent;
class USpringArmComponent;
class UPVZ3DHealthComponent;
class UPVZ3DInventoryComponent;
class APVZ3DWeapon;
class UPVZ3DWeaponComponent;

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DPlayer : public APVZ3DBaseEntity, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite,Category="Component")
	UPVZ3DInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
    UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* DeathAnimMontage;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Component")
	UPVZ3DWeaponComponent* WeaponComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAttacking = false;
	
public:
	APVZ3DPlayer();
	
	virtual void MoveForward(const float Val);
	virtual void MoveRight(const float Val);
	virtual void LookUp(const float Val);
	virtual void LookRight(const float Val);

	virtual void Attack();
	virtual void Interact();
	virtual void StartRun();
	virtual void StopRun();
	virtual void UpdateCurrentWeaponID();
	virtual void ChangeViewpoint();
	virtual void PlayerMouseClick();
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	void OnDeath();
	
	bool IsMovingForward = false;
	bool IsMovingRight = false;

	int CurrentWeaponID = 0;
	int AggroValue=0;
	int ViewType=0; // 0为第一人称，1为第三人称
	bool IsMouseInputDisabled = false; // 是否禁用鼠标输入
	FRotator LastCameraRotation; // 上次摄像机旋转角度
	FVector2D MovementInput;
	void UpdateMovement();

	FGenericTeamId TeamID;
	TArray<FGenericTeamId> FriendlyTeamIDs;
	TArray<FGenericTeamId> HostileTeamIDs;

	
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


	//判断移动角度
	UFUNCTION(BlueprintCallable,Category="Movement")
	float GetMovementDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopAttack();
};
