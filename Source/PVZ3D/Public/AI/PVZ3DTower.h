// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
<<<<<<< Updated upstream
=======
#include "Interface/PVZ3DInteractInterface.h"
#include "PVZ3D/CoreTypes/PVZ3DTowerCoreTypes.h" // 包含FTowerState定义
#include "Component/PVZ3DInventoryComponent.h"
#include "Interface/UPVZ3DTowerInterface.h" 
>>>>>>> Stashed changes
#include "PVZ3DTower.generated.h"

class UStaticMeshComponent;
class UPVZ3DHealthComponent;
class APVZ3DWeapon;
class UPVZ3DWeaponComponent;
class UCameraComponent;
<<<<<<< Updated upstream


UCLASS()
class PVZ3D_API APVZ3DTower : public APVZ3DBaseEntity,public IGenericTeamAgentInterface
=======
class UInventoryComponent;
class APVZ3DPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerWeaponChanged,FItemInInventory ,NewItem);


UCLASS()
class PVZ3D_API APVZ3DTower : public APVZ3DBaseEntity, public IGenericTeamAgentInterface, public IPVZ3DInteractInterface, public IPVZ3DTowerInterface
>>>>>>> Stashed changes
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned

<<<<<<< Updated upstream
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UStaticMeshComponent* TowerMeshComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="DataTable")
	UDataTable* TowerDataTable;

	
	
	
public:	
	// Called every frame
	APVZ3DTower();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Attack();
	virtual void Interact();
	virtual void UpdateCurrentWeaponID();//可以考虑被UpdateTowerImformation替代
	virtual void SwitchTower();
	virtual void UpdateTower();
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	
	FName CurrentWeaponID = "0002";
	int AggroValue=-1;
	double AttackRange=1000.0f;
	FRotator InitialOrientation;
	
	FGenericTeamId TeamID;
	TArray<FGenericTeamId> FriendlyTeamIDs;
	TArray<FGenericTeamId> HostileTeamIDs;
	FName TowerBehaviorTreeNow;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable,Category="AI")
	void UpdateTowerImformation();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* TowerBehaviorTree1;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* TowerBehaviorTree2;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* TowerBehaviorTree3;
=======
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UStaticMeshComponent* TowerMeshComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UPVZ3DHealthComponent* HealthComponent;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
    UDataTable* TowerDataTable;



public:
    APVZ3DTower();
    virtual void Tick(float DeltaTime) override;
    virtual void Attack();
    virtual void StopAttack();
    virtual void Interact(AActor* InstigatorActor) override;
    
    // 武器交换功能
    UFUNCTION(BlueprintCallable, Category = "TowerInteraction")
    void SwapWeaponsWithPlayer(APVZ3DPlayer* Player);
>>>>>>> Stashed changes

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* TowerBehaviorTree4;

<<<<<<< Updated upstream
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UCameraComponent* CameraComponent;
};
=======
    // 其他功能
    virtual void UpdateCurrentWeaponID();
    virtual void SwitchTower();
    virtual void UpdateTower();
    virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
    
    FName CurrentWeaponID = "0001";
    int AggroValue = -1;
    double AttackRange = 1000.0f;
    FRotator InitialOrientation;
    bool bIsAttacking;
    int AttackType;

    FOnTowerHealthChanged OnTowerHealthChanged;
    FPlayerWeaponChanged PlayerWeaponChanged;
    
    FGenericTeamId TeamID;
    TArray<FGenericTeamId> FriendlyTeamIDs;
    TArray<FGenericTeamId> HostileTeamIDs;
    FName TowerBehaviorTreeNow;
    
    // 团队相关
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
    virtual FGenericTeamId GetGenericTeamId() const override;
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

    UFUNCTION(BlueprintCallable, Category = "Home")
    void OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* TowerBehaviorTree1;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* TowerBehaviorTree2;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* TowerBehaviorTree3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* TowerBehaviorTree4;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UPVZ3DInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UPVZ3DWeaponComponent* WeaponComponent;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void BuildTower(FName NewWeaponID);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void TowerDied();


private:
    UPVZ3DInventoryComponent* TowerInventory;
    APVZ3DPlayer* Player;
    FTimerHandle InventoryCheckTimer;
    


public:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    // 实现接口方法
    virtual bool IsTowerValid_Implementation() const override;
    virtual bool HasWeapon_Implementation() const override;
    virtual bool IsTowerBaseInMiddle_Implementation() const override;
    virtual bool IsWeaponMaxLevel_Implementation() const override;

private:
    void StartInventoryCheckTimer();
    void StopInventoryCheckTimer();
    void CheckAndLogInventories();
    void LogTowerInventory();
    void LogPlayerInventory();

private:
    UPROPERTY(EditAnywhere, Category = "Tower")
    bool bIsBaseInMiddle = false;
    
    UPROPERTY(EditAnywhere, Category = "Tower")
    int32 CurrentLevel = 1;
    
    UPROPERTY(EditAnywhere, Category = "Tower")
    int32 MaxLevel = 3;

};
>>>>>>> Stashed changes
