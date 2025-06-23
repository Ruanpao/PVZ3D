// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskSetTargetNodeLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3D/Public/AI/PVZ3DRouteManager.h"
#include "PVZ3D/Public/AI/PVZ3DEnemy.h"


UPVZ3DTaskSetTargetNodeLocation::UPVZ3DTaskSetTargetNodeLocation()
{
	NodeName=TEXT("PVZ3DSetTargetNodeLocation");
}


EBTNodeResult::Type UPVZ3DTaskSetTargetNodeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
		if (Enemy)
		{
			int32 TargetIndex = BlackboardComp->GetValueAsInt(TargetNodeIndex.SelectedKeyName);

			TArray<TSoftObjectPtr<AActor>> SafeNodes;
			for (APVZ3DRouteNode* Node : Enemy->CurrentRouteNodes)
			{
				if (IsValid(Node)) SafeNodes.Add(Node);
			}
    
			if (!SafeNodes.IsValidIndex(TargetIndex))
			{
				return EBTNodeResult::Failed;
			}
			AActor* TargetRouteNode = SafeNodes[TargetIndex].Get();
			if (!IsValid(TargetRouteNode))
			{
				return EBTNodeResult::Failed;
			}
			if (TargetRouteNode)
			{
				FVector TargetLocation = TargetRouteNode->GetActorLocation();
				BlackboardComp->SetValueAsVector(TargetNode.SelectedKeyName, TargetLocation);

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
