// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/PVZ3DEnemyFindTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/PVZ3DEnemyPerceptionComponent.h"


UPVZ3DEnemyFindTargetService::UPVZ3DEnemyFindTargetService()
{
	NodeName = TEXT("Enemy Find Target");
}

void UPVZ3DEnemyFindTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard=OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller=OwnerComp.GetAIOwner();
		const auto PerceptionComponent = Controller->FindComponentByClass<UPVZ3DEnemyPerceptionComponent>();
		if(PerceptionComponent)
		{
			Blackboard->SetValueAsObject(TargetKey.SelectedKeyName,PerceptionComponent->GetLastSeenTarget());
			//UE_LOG(LogTemp,Warning,TEXT("PerceptionComponent"));
			if(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName))
            {
                //UE_LOG(LogTemp,Warning,TEXT("Target:%s"),*Blackboard->GetValueAsObject(TargetKey.SelectedKeyName)->GetName());
            }
            else
            {
                //UE_LOG(LogTemp,Warning,TEXT("Target is null"));
            }
		}
	}
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

		
}
