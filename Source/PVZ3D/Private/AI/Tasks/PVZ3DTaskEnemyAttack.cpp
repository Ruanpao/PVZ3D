// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskEnemyAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3DHealthComponent.h"
#include "PVZ3D/Public/AI/PVZ3DEnemy.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Engine/DamageEvents.h"

UPVZ3DTaskEnemyAttack::UPVZ3DTaskEnemyAttack()
{
	NodeName = TEXT("Enemy Attack Target");
}
EBTNodeResult::Type UPVZ3DTaskEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    // 获取黑板中的攻击目标
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) 
    {
        return EBTNodeResult::Failed;
    }

    CurrentTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!CurrentTarget) 
    {
        return EBTNodeResult::Failed;
    }

    // 立即执行首次攻击
    UE_LOG(LogTemp, Warning, TEXT("111Enemy %s is attacking target %s"), *OwnerComp.GetAIOwner()->GetPawn()->GetName(), *CurrentTarget->GetName());
    OnAttackTimerElapsed();

    // 启动循环定时器
    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle, 
        this, 
        &UPVZ3DTaskEnemyAttack::OnAttackTimerElapsed, 
        AttackInterval, 
        true // 循环执行
    );

    return EBTNodeResult::InProgress; // 任务保持运行状态
}

void UPVZ3DTaskEnemyAttack::OnAttackTimerElapsed()
{
    if (!OwnerCompRef || !CurrentTarget)
    {
        ClearAttackTimer();
        UE_LOG(LogTemp, Warning, TEXT("Enemy Attack Task: OwnerCompRef or CurrentTarget is null, clearing timer."));
        return;
    }

    // 获取敌人和AI控制器
    AAIController* AIController = OwnerCompRef->GetAIOwner();
    APVZ3DEnemy* Enemy = AIController ? Cast<APVZ3DEnemy>(AIController->GetPawn()) : nullptr;
    if (!Enemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy Attack Task: Enemy is null, clearing timer."));
        ClearAttackTimer();
        return;
    }

    // 计算与目标的距离
    float DistanceToTarget = FVector::Dist(Enemy->GetActorLocation(), CurrentTarget->GetActorLocation());
    
    // 检查是否在攻击范围内（假设AttackRange是APVZ3DEnemy的属性）
    if (DistanceToTarget > Enemy->AttackRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy %s is out of range (%.2f > %.2f), cannot attack"), 
               *Enemy->GetName(), DistanceToTarget, Enemy->AttackRange);
        return; // 不在范围内，不执行攻击
    }

    // 执行攻击动作
    Enemy->Attack();
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s is attacking target %s (range: %.2f)"), 
           *Enemy->GetName(), *CurrentTarget->GetName(), DistanceToTarget);

    // 对目标造成伤害
    UPVZ3DHealthComponent* TargetHealthComp = CurrentTarget->FindComponentByClass<UPVZ3DHealthComponent>();
    if (TargetHealthComp)
    {
        float DamageAmount = Enemy->AttackDamage;
        CurrentTarget->TakeDamage(DamageAmount, FDamageEvent(), AIController, Enemy);

        UE_LOG(LogTemp, Warning, TEXT("Enemy %s dealt %.2f damage to target %s"), 
               *Enemy->GetName(), DamageAmount, *CurrentTarget->GetName());
        
        if (TargetHealthComp->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("Enemy %s killed target %s"), *Enemy->GetName(), *CurrentTarget->GetName());
            ClearAttackTimer();
        }
    }
}

void UPVZ3DTaskEnemyAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result)
{
    ClearAttackTimer(); // 清理定时器
    Super::OnTaskFinished(OwnerComp, NodeMemory, Result);
}

void UPVZ3DTaskEnemyAttack::ClearAttackTimer()
{
    if (GetWorld() && AttackTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    }
}