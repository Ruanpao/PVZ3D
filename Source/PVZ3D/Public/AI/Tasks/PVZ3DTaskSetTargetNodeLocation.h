// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskSetTargetNodeLocation.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskSetTargetNodeLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPVZ3DTaskSetTargetNodeLocation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetNodeIndex;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetNode;
	
};
