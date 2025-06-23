// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskGetInitialRotation.generated.h"

/**
 *  从Tower中获取InitialRotation并应用到Tower上
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskGetInitialRotation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPVZ3DTaskGetInitialRotation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetKey;
	

	
	
};
