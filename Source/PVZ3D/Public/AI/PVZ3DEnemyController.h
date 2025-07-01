// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PVZ3DEnemyController.generated.h"

/**
 * 
 */
class UPVZ3DEnemyPerceptionComponent;

UCLASS()
class PVZ3D_API APVZ3DEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	APVZ3DEnemyController();
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TArray<FGenericTeamId> FriendlyTeamID;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TArray<FGenericTeamId> HostileTeamID;

	UPROPERTY(visibleAnywhere, Category = "Component")
	UPVZ3DEnemyPerceptionComponent* EnemyAIPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetKeyName = "Target";
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable,Category="AI")
	void RunPVZ3DEnemyBehaviorTree(FName BehaviorTreeID);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdateEnemyControllerinformation();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopBehaviorTree();

	bool IsDeath=false;
private:
	AActor* GetTargetActor() const;
};
