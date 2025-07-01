// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskChangeSunAttackState.h"
#include "AI/PVZ3DTower.h"
#include "AI/PVZ3DTowerController.h"

UPVZ3DTaskChangeSunAttackState::UPVZ3DTaskChangeSunAttackState()
{
	NodeName = TEXT("Chanege Sunflower Attack State");
}

EBTNodeResult::Type UPVZ3DTaskChangeSunAttackState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(BlackboardComp)
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
		if(TargetActor)
		{
			APVZ3DTower* Tower = Cast<APVZ3DTower>(OwnerComp.GetAIOwner()->GetPawn());
			
			if(Tower->bIsAttacking)
			{
				Tower->StopAttack();
				Tower->bIsAttacking = false;
				UE_LOG(LogTemp, Warning, TEXT("CHANGED Tower Stop Attack Target: %s"), *TargetActor->GetName());
				return EBTNodeResult::Succeeded;
			}
		}
	}

	
	return EBTNodeResult::Succeeded;
}
