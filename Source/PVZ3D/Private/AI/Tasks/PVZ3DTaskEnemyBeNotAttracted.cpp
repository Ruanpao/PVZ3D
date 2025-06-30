// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskEnemyBeNotAttracted.h"
#include "AI/PVZ3DEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UPVZ3DTaskEnemyBeNotAttracted::UPVZ3DTaskEnemyBeNotAttracted()
{
	NodeName = TEXT("PVZ3DEnemyBeNotAttracted");

}

EBTNodeResult::Type UPVZ3DTaskEnemyBeNotAttracted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->BeAttracted = false;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
