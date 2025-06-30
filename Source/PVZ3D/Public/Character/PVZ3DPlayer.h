// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/PVZ3DInteractInterface.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "PVZ3DPlayer.generated.h"



class UCameraComponent;
class USpringArmComponent;
class UPVZ3DHealthComponent;
class UPVZ3DInventoryComponent;
class APVZ3DWeapon;
class UPVZ3DWeaponComponent;
class APVZ3DPlayerSpawnPoint;
class APVZ3DTower;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Alive,         // 存活
	Dying,         // 死亡动画播放中
	Spectating,    // 旁观者模式
	Respawning     // 复活中
};

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, APlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnTowerInteraction, bool, bTowerHasWeapon, bool, bTowerBaseInMiddle, bool, bTowerWeaponMaxLevel, bool, bIsNearTower, APVZ3DTower*, Tower);


DECLARE_DELEGATE(FSwitchToStack1);
DECLARE_DELEGATE(FSwitchToStack2);
DECLARE_DELEGATE(FSwitchToStack3);
DECLARE_DELEGATE(FSwitchToStack4);
DECLARE_DELEGATE(FSwitchToStack5);
DECLARE_DELEGATE(FSwitchToStack6);
DECLARE_DELEGATE(FSwitchToStack7);
DECLARE_DELEGATE(FSwitchToStack8);
DECLARE_DELEGATE(FSwitchToStack9);
DECLARE_DELEGATE(FSwitchToStack10);


UCLASS()
class PVZ3D_API APVZ3DPlayer : public APVZ3DBaseEntity, public IGenericTeamAgentInterface, public IPVZ3DInteractInterface
{
	GENERATED_BODY()


private:
	FTimerHandle TimerHandle_Respawn; // 复活计时器
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
    UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* DeathAnimMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsRunning = false;  

	UPROPERTY(EditAnywhere, Category = "Movement")
	float NormalSpeed = 1000.0f;  

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunningSpeed = 2000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	APVZ3DPlayerSpawnPoint *SpawnPoint;
	
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
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void OnDeath();

	void OnHoldedItemChanged(FItemInInventory Item);

	void SwitchToStack1();
	void SwitchToStack2();
	void SwitchToStack3();
	void SwitchToStack4();
	void SwitchToStack5();
	void SwitchToStack6();
	void SwitchToStack7();
	void SwitchToStack8();
	void SwitchToStack9();
	void SwitchToStack10();
	
	bool IsMovingForward = false;
	bool IsMovingRight = false;
	
	FName CurrentWeaponID;
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

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SwapWeaponsWithTower(APVZ3DTower* Tower);
	//判断移动角度
	UFUNCTION(BlueprintCallable,Category="Movement")
	float GetMovementDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const { return bIsRunning; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDied OnPlayerDied;

	UFUNCTION(BlueprintCallable, Category="Player")
	UPVZ3DWeaponComponent* GetWeaponComponent() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Respawn")
	EPlayerState CurrentState = EPlayerState::Alive;

	UPROPERTY(VisibleAnyWhere,BlueprintReadWrite,Category="Component")
	UPVZ3DInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Component")
	UPVZ3DWeaponComponent* WeaponComponent;

	UPROPERTY(Transient) float SavedHealth; // 缓存血量
	//UPROPERTY(Transient) TArray<FInventoryItemData> SavedInventory; // 缓存物品
	UPROPERTY(Transient) FVector DeathLocation; // 缓存死亡位置
	UPROPERTY(Transient) FRotator DeathRotation; // 缓存死亡旋转
	UPROPERTY(Transient) FVector SpawnPointLocation; // 缓存复活点位置

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay = 5.0f; // 复活延迟
	
	//UPROPERTY(EditDefaultsOnly, Category = "Animation")
	//UAnimMontage* RespawnAnimMontage; // 复活动画

	// 新增：复活相关函数
	UFUNCTION() void OnDeathInitiated(); // 触发死亡（替代原OnDeath逻辑）
	UFUNCTION() void EnterSpectatorMode(); // 切换到旁观者
	UFUNCTION() void ExitSpectatorMode(); // 退出旁观者
	UFUNCTION() void StartRespawnTimer(); // 启动复活计时器
	UFUNCTION() void RespawnCharacter(); // 执行复活

	FTimerHandle RespawnTimerHandle; // 复活计时器

	APVZ3DTower* FindNearestTowerInRange();

	// 广播塔的信息
	void BroadcastTowerInfo(APVZ3DTower* Tower);
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TSubclassOf<AActor> TowerClass;

	// 存储塔信息的变量
	bool bTowerHasWeapon;
	bool bTowerBaseInMiddle;
	bool bTowerWeaponMaxLevel;
	bool bIsNearTower;
	APVZ3DTower* CurrentTower;

	// 委托
	UPROPERTY(BlueprintAssignable, Category = "TowerInteraction")
	FOnTowerInteraction OnTowerInteraction;

	// 交互范围
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionRange = 200.0f;
	
};
