// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskUpdateCurrentTargetNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3D/Public/AI/PVZ3DRouteManager.h"
#include "PVZ3D/Public/AI/PVZ3DEnemy.h"


UPVZ3DTaskUpdateCurrentTargetNode::UPVZ3DTaskUpdateCurrentTargetNode()
{
	NodeName = "PVZ3DUpdateCurrentTargetNode";
}

EBTNodeResult::Type UPVZ3DTaskUpdateCurrentTargetNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
		if (Enemy)
		{
			APVZ3DRouteManager* RouteManager = Cast<APVZ3DRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APVZ3DRouteManager::StaticClass()));
			if (RouteManager)
			{
				// 获取当前路径的节点数组
				int RouteID = Enemy->RouteID;
				TArray<APVZ3DRouteNode*> RouteNodes = RouteManager->GetRouteNodesByID(RouteID);
				
				int CurrentTargetNodeIndex = BlackboardComp->GetValueAsInt(TargetNodeIndexKey.SelectedKeyName);
				
				if (CurrentTargetNodeIndex < RouteNodes.Num())
				{
					int NextTargetNodeIndex = (CurrentTargetNodeIndex + 1) % RouteNodes.Num();
					BlackboardComp->SetValueAsInt(TargetNodeIndexKey.SelectedKeyName, NextTargetNodeIndex);
					return EBTNodeResult::Succeeded;
				}
    
				// 获取安全的路径点数组
				return EBTNodeResult::Failed;	


				
			}
		}
	}

	return EBTNodeResult::Failed;
}