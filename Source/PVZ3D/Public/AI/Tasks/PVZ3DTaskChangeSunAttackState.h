// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskChangeSunAttackState.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskChangeSunAttackState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPVZ3DTaskChangeSunAttackState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetKey;
	
};
