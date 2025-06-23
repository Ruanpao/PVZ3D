// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/PVZ3DTowerFindTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/PVZ3DTowerPerceptionComponent.h"

UPVZ3DTowerFindTargetService::UPVZ3DTowerFindTargetService()
{
	NodeName= TEXT("Tower Find Target");
}

void UPVZ3DTowerFindTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard=OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller=OwnerComp.GetAIOwner();
		const auto PerceptionComponent = Controller->FindComponentByClass<UPVZ3DTowerPerceptionComponent>();
		if(PerceptionComponent)
		{
			Blackboard->SetValueAsObject(TargetKey.SelectedKeyName,PerceptionComponent->GetLastSeenTarget());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
