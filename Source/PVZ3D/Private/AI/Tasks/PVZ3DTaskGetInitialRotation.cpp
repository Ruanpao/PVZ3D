// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskGetInitialRotation.h"
#include "AI/PVZ3DTower.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"



UPVZ3DTaskGetInitialRotation::UPVZ3DTaskGetInitialRotation()
{
	NodeName = TEXT("Apply Initial Rotation");
}

EBTNodeResult::Type UPVZ3DTaskGetInitialRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(BlackboardComp)
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
		if(TargetActor)
		{
			APVZ3DTower* Tower = Cast<APVZ3DTower>(OwnerComp.GetAIOwner()->GetPawn());
			if(Tower)
			{
				Tower->SetActorRotation(Tower->InitialOrientation);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	
	return EBTNodeResult::Failed;
}
