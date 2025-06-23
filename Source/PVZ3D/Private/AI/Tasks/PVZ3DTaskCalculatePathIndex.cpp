// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskCalculatePathIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3D/Public/AI/PVZ3DRouteManager.h"
#include "PVZ3D/Public/AI/PVZ3DEnemy.h"
#include "PVZ3D/Public/AI/PVZ3DRouteNode.h"
#include "NavigationSystem.h"


UPVZ3DTaskCalculateOptimalPathIndex::UPVZ3DTaskCalculateOptimalPathIndex()
{
    NodeName = TEXT("PVZ3DCalculatePathIndex");

}

FVector UPVZ3DTaskCalculateOptimalPathIndex::GetClosestPointOnSegment(
    const FVector& P, const FVector& A, const FVector& B) const
{
    const FVector AB = B - A;
    const float MagnitudeAB = AB.SizeSquared();
    if (MagnitudeAB < KINDA_SMALL_NUMBER) return A;

    const FVector AP = P - A;
    const float t = FMath::Clamp(FVector::DotProduct(AP, AB) / MagnitudeAB, 0.0f, 1.0f);
    return A + t * AB;
}





EBTNodeResult::Type UPVZ3DTaskCalculateOptimalPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APVZ3DRouteManager* RouteManager = Cast<APVZ3DRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APVZ3DRouteManager::StaticClass()));

    APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (!IsValid(Enemy)) return EBTNodeResult::Failed;

    if (RouteManager)
    {
        // 获取当前路径的节点数组
        int RouteID = Enemy->RouteID;
        TArray<APVZ3DRouteNode*> RouteNodes = RouteManager->GetRouteNodesByID(RouteID);

        UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
        if (!BlackboardComp) return EBTNodeResult::Failed;



        const int32 OriginalIndex = BlackboardComp->GetValueAsInt(TargetNodeIndexKey.SelectedKeyName);
    
        // 获取当前索引并检查范围
        if (!RouteNodes.IsValidIndex(OriginalIndex))
        {
            BlackboardComp->SetValueAsInt(TargetNodeIndexKey.SelectedKeyName, 0);
            return EBTNodeResult::Succeeded;
        }

        const FVector CurrentLocation = Enemy->GetActorLocation();
        float MinDistance = FLT_MAX;
        int32 BestSegmentEndIndex = OriginalIndex;

        // 只检查后续路径线段
        for (int32 i = OriginalIndex; i < RouteNodes.Num() - 1; ++i)
        {
            AActor* StartNode = RouteNodes[i];
            AActor* EndNode = RouteNodes[i+1];
        
            if (!IsValid(StartNode) || !IsValid(EndNode)) continue;
        
            const FVector StartLoc = StartNode->GetActorLocation();
            const FVector EndLoc = EndNode->GetActorLocation();
            const FVector ClosestPoint = GetClosestPointOnSegment(CurrentLocation, StartLoc, EndLoc);
        
            const float CurrentDist = FVector::Distance(CurrentLocation, ClosestPoint);
            if (CurrentDist < MinDistance)
            {
                MinDistance = CurrentDist;
                BestSegmentEndIndex = i + 1; // 选择线段终点的索引
            }
        }

        // 确保最终索引有效
        if (BestSegmentEndIndex < RouteNodes.Num() && IsValid(RouteNodes[BestSegmentEndIndex]))
        {
            BlackboardComp->SetValueAsInt(TargetNodeIndexKey.SelectedKeyName, BestSegmentEndIndex);
        }
        else
        {
            // 回退到原始索引
            BlackboardComp->SetValueAsInt(TargetNodeIndexKey.SelectedKeyName, OriginalIndex);
        }
    }
    
    
    return EBTNodeResult::Succeeded;
}
