// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PVZ3DTower.generated.h"

class UStaticMeshComponent;
class UPVZ3DHealthComponent;
class APVZ3DWeapon;
class UPVZ3DWeaponComponent;
class UCameraComponent;


UCLASS()
class PVZ3D_API APVZ3DTower : public APVZ3DBaseEntity,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned

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

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* TowerBehaviorTree4;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UCameraComponent* CameraComponent;
};
