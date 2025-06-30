// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskAttackTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3DHealthComponent.h"
#include "AI/PVZ3DTower.h"
#include "Camera/CameraComponent.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"

UPVZ3DTaskAttackTarget::UPVZ3DTaskAttackTarget()
{
	NodeName = TEXT("Attack Target");
    AttackInterval = 2.0f;  // 设置默认攻击间隔为2秒
    CurrentTime = 0.0f;
    bNotifyTick = true;  // 启用Tick功能，这样TickTask函数会被调用
}

EBTNodeResult::Type UPVZ3DTaskAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
        if (TargetActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Target found: %s"), *TargetActor->GetName());
            return EBTNodeResult::InProgress;  // 任务开始，设置为进行中状态
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Target not found or invalid!"));
    return EBTNodeResult::Failed;  // 获取目标失败，任务失败
}

void UPVZ3DTaskAttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
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
                UpdateTowerRotation(Tower, TargetActor);  // 更新Tower的朝向

                CurrentTime += DeltaTime;  // 更新当前计时器时间
                if (CurrentTime >= AttackInterval)  // 如果计时器时间达到攻击间隔
                {
                    Tower->Attack();  // 执行攻击动作
                    //UE_LOG(LogTemp, Warning, TEXT("Tower Attack Target: %s"), *TargetActor->GetName());

                    // 对目标应用伤害
                    // UPVZ3DHealthComponent* TargetHealthComponent = TargetActor->FindComponentByClass<UPVZ3DHealthComponent>();
                    // if (TargetHealthComponent)
                    // {
                    //     float DamageAmount = 10.0f; 
                    //     AActor* DamageCauser = Tower;
                    //     AController* InstigatedBy = Tower->GetController();
                    //     UDamageType const* DamageType = nullptr;
                    //     TargetActor->TakeDamage(DamageAmount, FDamageEvent(), InstigatedBy, DamageCauser);
                    // }

                    CurrentTime = 0.0f;  // 重置计时器
                }
            }
        }
    }
}

void UPVZ3DTaskAttackTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    CurrentTime = 0.0f;  // 任务结束，重置计时器
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);  // 调用父类的OnTaskFinished函数
}


void UPVZ3DTaskAttackTarget::UpdateTowerRotation(APVZ3DTower* Tower, AActor* TargetActor)
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
