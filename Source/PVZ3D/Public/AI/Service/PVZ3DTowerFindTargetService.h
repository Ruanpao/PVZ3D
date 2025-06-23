// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PVZ3DTowerFindTargetService.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTowerFindTargetService : public UBTService
{
	GENERATED_BODY()

public:
	UPVZ3DTowerFindTargetService();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	FBlackboardKeySelector TargetKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
