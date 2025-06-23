// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskEnemyBeAttracted.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskEnemyBeAttracted : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPVZ3DTaskEnemyBeAttracted();
    
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
