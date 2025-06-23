// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskTowerRotateAlert.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "CoreMinimal.h"
#include "AI/PVZ3DTower.h"
#include "AI/PVZ3DTowerController.h"
#include "AI/PVZ3DTowerPerceptionComponent.h"
#include "Math/UnrealMathUtility.h"

UPVZ3DTaskTowerRotateAlert::UPVZ3DTaskTowerRotateAlert()
{
	NodeName = TEXT("Rotate Alert");
	CurrentRotationAngle = 0.0f;
	RotationSpeed = 0.10f;  // 旋转速度，度/秒
	MaxRotationAngle = 30.0f;  // 最大旋转角度
	bIsRotatingLeft = true;   // 初始时向左旋转
	bNotifyTick = true;
}

EBTNodeResult::Type UPVZ3DTaskTowerRotateAlert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AActor* TowerActor = OwnerComp.GetAIOwner()->GetPawn();
	
	return EBTNodeResult::InProgress;  // 任务开始，设置为进行中状态

	
}

void UPVZ3DTaskTowerRotateAlert::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	AActor* TowerActor = OwnerComp.GetAIOwner()->GetPawn();
	if (TowerActor)
	{
		APVZ3DTower* Tower = Cast<APVZ3DTower>(TowerActor);
		if (Tower)
		{
			//float RotationDelta = RotationSpeed;
			FRotator NewRotation = Tower->GetActorRotation();
			if (bIsRotatingLeft)
   			{
   				CurrentRotationAngle -= RotationSpeed;
				
   				NewRotation.Yaw -= RotationSpeed;
   				
   				if (CurrentRotationAngle<= -MaxRotationAngle)
   				{
   					bIsRotatingLeft = false;
   				}
   			}
   			else
   			{
   				CurrentRotationAngle += RotationSpeed;
   				
   				NewRotation.Yaw += RotationSpeed;
   	
   				if (CurrentRotationAngle >= MaxRotationAngle)
   				{
   					bIsRotatingLeft = true;
   				}
   			}
			Tower->SetActorRotation(NewRotation);
   			
    	}
	}
    	
}


void UPVZ3DTaskTowerRotateAlert::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 任务结束时的处理，这里简单处理为重置旋转角度
	CurrentRotationAngle = 0.0f;
	UE_LOG(LogTemp, Warning, TEXT("Tower Rotation Alert Finished"));
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}
