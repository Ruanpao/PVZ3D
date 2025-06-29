// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskTowerSunFlowerAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3DHealthComponent.h"
#include "AI/PVZ3DTower.h"
#include "Camera/CameraComponent.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"

UPVZ3DTaskTowerSunFlowerAttack::UPVZ3DTaskTowerSunFlowerAttack()
{
	NodeName = TEXT("Tower SunFlower Attack");
	AttackInterval = 0.0f;  // 设置默认攻击间隔为2秒
	CurrentTime = 0.0f;
	bNotifyTick = true;  // 启用Tick功能，这样TickTask函数会被调用
}

EBTNodeResult::Type UPVZ3DTaskTowerSunFlowerAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
        if (TargetActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Target found: %s"), *TargetActor->GetName());
            return EBTNodeResult::InProgress;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Target not found or invalid!"));
    return EBTNodeResult::Failed;
}

void UPVZ3DTaskTowerSunFlowerAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
        if (TargetActor)
        {
            APVZ3DTower* Tower = Cast<APVZ3DTower>(OwnerComp.GetAIOwner()->GetPawn());
            if (Tower)
            {
                UpdateTowerRotation(Tower, TargetActor);
                
      
            	if (!Tower->bIsAttacking)
            	{Tower->bIsAttacking = true;
            		Tower->Attack();
            		UE_LOG(LogTemp, Warning, TEXT("Tower Laser Attacking Target: %s"), *TargetActor->GetName());
            	}

                
            }
        }
        else
        {
        }
    }
}

void UPVZ3DTaskTowerSunFlowerAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    // 任务结束时停止攻击
    APVZ3DTower* Tower = Cast<APVZ3DTower>(OwnerComp.GetAIOwner()->GetPawn());
    if (Tower && Tower->bIsAttacking)
    {
        Tower->StopAttack();
    }
    
    CurrentTime = 0.0f;
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UPVZ3DTaskTowerSunFlowerAttack::UpdateTowerRotation(APVZ3DTower* Tower, AActor* TargetActor)
{
	if (Tower && TargetActor && Tower->CameraComponent)
	{
		FVector Direction = TargetActor->GetActorLocation() - Tower->CameraComponent->GetComponentLocation();
        
		if (Direction.SizeSquared() > KINDA_SMALL_NUMBER)  // 确保方向向量有效
		{
			FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			Direction.Z = 0;
			FRotator NewTowerRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			Tower->SetActorRotation(NewTowerRotation);  // 设置Tower的旋转

			FRotator CameraRotation = Tower->CameraComponent->GetComponentRotation();
			CameraRotation.Pitch = NewRotation.Pitch;  // 设置摄像机的Yaw旋转
			Tower->CameraComponent->SetWorldRotation(CameraRotation);
            
		}
	}
}
