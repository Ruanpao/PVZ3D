// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PVZ3DRouteManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/PVZ3DBaseEntity.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PVZ3DEnemy.generated.h"



class UPVZ3DHealthComponent;
class UTextRenderComponent;
/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DEnemy : public APVZ3DBaseEntity, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APVZ3DEnemy();
	
protected:
    	// Called when the game starts or when spawned
   	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	UPVZ3DHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")	//测试血量
	UTextRenderComponent* HealthTextComponent;

public:	
    	// Called every frame
   	virtual void Tick(float DeltaTime) override;
    
   	virtual void Attack();
   	virtual void Interact();
   	virtual void StartRun();
   	virtual void StopRun();
   	virtual void UpdateCurrentWeaponID();
    	
   	bool IsMovingForward = false;
	bool IsMovingRight = false;
	bool IsDead = false;
	FName CurrentWeaponID;
	int AggroValue=0;
	int CurrentNodeIndex=0;
	int Vecolity=100;
	bool BeAttracted=false;
	float AttackRange;
	float AttackDamage;
	float AttackInterval;

	FGenericTeamId TeamID;
	TArray<FGenericTeamId> FriendlyTeamIDs;
	TArray<FGenericTeamId> HostileTeamIDs;
	
	APVZ3DRouteManager* RouteManager;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	int RouteID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	FName EnemyBehaviorTreeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyID;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Route")
	TArray<APVZ3DRouteNode*> CurrentRouteNodes;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* EnemyBehaviorTree1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* EnemyBehaviorTree2;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	UBehaviorTree* EnemyBehaviorTree3;
	
	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* DeathAnimMontage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="DataTable")
	UDataTable* EnemyDataTable;
	
	UFUNCTION(BlueprintCallable,Category="Route")
	virtual void SetRouteID(int NewRouteID);

	UFUNCTION(BlueprintCallable,Category="Route")
	virtual void MoveAlongRoute();

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable,Category="Movement")
	float GetMovementDirection() const;

	UFUNCTION(BlueprintCallable,Category="AI")
	void UpdateEnemyImformation();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateEnemy();
	
private:

	void OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent);
	
	void OnDeath();
};
