// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PVZ3DTaskAttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTaskAttackTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPVZ3DTaskAttackTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;  // 添加TickTask函数声明，用于每帧更新
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;  // 添加OnTaskFinished函数声明，用于任务结束时的操作

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	FBlackboardKeySelector TargetKey;

	float AttackInterval; // 攻击间隔时间
	float CurrentTime;    // 当前计时器时间
	void UpdateTowerRotation(AActor* Tower, AActor* TargetActor);  // 用于更新Tower朝向Target的函数声明

};
