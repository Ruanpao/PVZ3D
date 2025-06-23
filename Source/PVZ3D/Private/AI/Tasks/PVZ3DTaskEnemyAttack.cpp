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
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
        if (TargetActor)
        {
            APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
            if (Enemy)
            {
                Enemy->Attack();
                UE_LOG(LogTemp, Warning, TEXT("Enemy Attack Target: %s"), *TargetActor->GetName());

                // 对目标应用伤害
                UPVZ3DHealthComponent* TargetHealthComponent = TargetActor->FindComponentByClass<UPVZ3DHealthComponent>();
                if (TargetHealthComponent)
                {
                    // 假设这里的伤害值是一个固定值，你可以根据实际情况调整
                    float DamageAmount = 10.0f; 
                    AActor* DamageCauser = Enemy;
                    AController* InstigatedBy = Enemy->GetController();
                    UDamageType const* DamageType = nullptr;
                    TargetActor->TakeDamage(DamageAmount, FDamageEvent(), InstigatedBy, DamageCauser);
                }

                return EBTNodeResult::Succeeded;
            }
            return EBTNodeResult::Failed;
        }
    }
    return EBTNodeResult::Failed;
}
