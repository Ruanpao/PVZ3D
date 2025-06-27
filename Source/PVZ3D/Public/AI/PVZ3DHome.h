// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "GenericTeamAgentInterface.h"
#include "PVZ3DHome.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class UPVZ3DHealthComponent;
// class APVZ3DWeapon;
// class UPVZ3DWeaponComponent;
class UCameraComponent;
class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHomeDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHomeHealthChanged, float, NewHealth);

UCLASS()
class PVZ3D_API APVZ3DHome : public APVZ3DBaseEntity,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UStaticMeshComponent* HomeMeshComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")	//测试血量
	UTextRenderComponent* HealthTextComponent;

	// UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	// UPVZ3DWeaponComponent* WeaponComponent;

	
public:	
	// Called every frame
	APVZ3DHome();
	
	virtual void Tick(float DeltaTime) override;
	//virtual void Attack();
	virtual void Interact();
	//virtual void UpdateCurrentWeaponID();//可以考虑被UpdateTowerImformation替代
	// virtual void SwitchTower();
	// virtual void UpdateTower();
	 virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	
	//FName CurrentWeaponID = "0002";
	int AggroValue=-1;
	float HomeMaxHealth=1000.0f;
	//double AttackRange=1000.0f;
	//FRotator InitialOrientation;
	
	FGenericTeamId TeamID;
	TArray<FGenericTeamId> FriendlyTeamIDs;
	TArray<FGenericTeamId> HostileTeamIDs;
	//FName TowerBehaviorTreeNow;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "Home")
	void OnHomeDeath();

	UFUNCTION(BlueprintCallable, Category = "Home")
	void OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent);

	UFUNCTION(BlueprintCallable, Category = "Home")
	float GetHomeCurrentHealth();
	
	// UFUNCTION(BlueprintCallable,Category="AI")
	// void UpdateTowerImformation();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FHomeDeathDelegate HomeDeathDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Home")
	FOnHomeHealthChanged OnHomeHealthChanged;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Events")
	AGameModeBase* Gamemode;
};
