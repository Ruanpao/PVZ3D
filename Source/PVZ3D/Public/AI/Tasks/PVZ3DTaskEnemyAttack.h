// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskEnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskEnemyAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPVZ3DTaskEnemyAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackInterval = 5.0f;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type Result) override;

private:
	// 定时器句柄
	FTimerHandle AttackTimerHandle;
	// 保存行为树组件引用
	UBehaviorTreeComponent* OwnerCompRef = nullptr;
	// 当前攻击目标
	AActor* CurrentTarget = nullptr;

	// 定时器回调：执行攻击逻辑
	void OnAttackTimerElapsed();
	// 清理定时器
	void ClearAttackTimer();

	
};
