// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskEnemyBeNotAttracted.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskEnemyBeNotAttracted : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPVZ3DTaskEnemyBeNotAttracted();
    
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
