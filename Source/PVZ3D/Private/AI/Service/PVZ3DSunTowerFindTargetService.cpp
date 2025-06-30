// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/PVZ3DSunTowerFindTargetService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/PVZ3DTowerPerceptionComponent.h"

UPVZ3DSunTowerFindTargetService::UPVZ3DSunTowerFindTargetService()
{
	NodeName = TEXT("Tower Sun Find Target");
}

void UPVZ3DSunTowerFindTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard=OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller=OwnerComp.GetAIOwner();
		const auto PerceptionComponent = Controller->FindComponentByClass<UPVZ3DTowerPerceptionComponent>();
		if(PerceptionComponent)
		{
			Blackboard->SetValueAsObject(TargetKey.SelectedKeyName,PerceptionComponent->SunFlowerGetLastSeenTarget());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
