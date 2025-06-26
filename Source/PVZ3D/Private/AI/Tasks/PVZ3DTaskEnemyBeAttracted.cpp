// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskEnemyBeAttracted.h"

#include "AI/PVZ3DEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UPVZ3DTaskEnemyBeAttracted::UPVZ3DTaskEnemyBeAttracted()
{
	NodeName = TEXT("PVZ3DEnemyBeNotAttracted");

}

EBTNodeResult::Type UPVZ3DTaskEnemyBeAttracted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->BeAttracted = true;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
