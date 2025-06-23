// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PVZ3DSunTowerFindTargetService.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DSunTowerFindTargetService : public UBTService
{
	GENERATED_BODY()

public:
	UPVZ3DSunTowerFindTargetService();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	FBlackboardKeySelector TargetKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
