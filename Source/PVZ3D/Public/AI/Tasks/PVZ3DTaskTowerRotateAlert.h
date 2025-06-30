// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskTowerRotateAlert.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskTowerRotateAlert : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPVZ3DTaskTowerRotateAlert();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	FBlackboardKeySelector TargetKey;

	float CurrentRotationAngle;
	float RotationSpeed;
	float MaxRotationAngle;
	bool bIsRotatingLeft;
};
