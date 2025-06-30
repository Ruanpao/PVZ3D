// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/PVZ3DTaskSetTargetNodeLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PVZ3D/Public/AI/PVZ3DRouteManager.h"
#include "PVZ3D/Public/AI/PVZ3DEnemy.h"


UPVZ3DTaskSetTargetNodeLocation::UPVZ3DTaskSetTargetNodeLocation()
{
	NodeName=TEXT("PVZ3DSetTargetNodeLocation");
}


EBTNodeResult::Type UPVZ3DTaskSetTargetNodeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APVZ3DRouteManager* RouteManager = Cast<APVZ3DRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APVZ3DRouteManager::StaticClass()));
	APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy)) return EBTNodeResult::Failed;
	int RouteID = Enemy->RouteID;
	//UE_LOG(LogTemp, Warning, TEXT("UPVZ3DTaskSetTargetNodeLocation: Enemy RouteID: %d"), RouteID);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!IsValid(BlackboardComp)) return EBTNodeResult::Failed;
	const FVector CurrentLocation = Enemy->GetActorLocation();
	const int32 OriginalIndex = BlackboardComp->GetValueAsInt(TargetNodeIndex.SelectedKeyName);
	const int32 TargetIndex = BlackboardComp->GetValueAsInt(TargetNodeIndex.SelectedKeyName);
	float MinDistance = FLT_MAX;
	int32 BestSegmentEndIndex = OriginalIndex;

	TArray<APVZ3DRouteNode*> SafeNodes;
	
	for (APVZ3DRouteNode* Node : Enemy->CurrentRouteNodes)
	{
		if (IsValid(Node))
		{
			SafeNodes.Add(Node);
		}
	}
	
	if(!Enemy->BeAttracted)
	{
		if (!SafeNodes.IsValidIndex(TargetIndex))
		{
			return EBTNodeResult::Failed;
		}
		AActor* TargetRouteNode = SafeNodes[TargetIndex];
		if (!IsValid(TargetRouteNode))
		{
			return EBTNodeResult::Failed;
		}

		if (TargetRouteNode)
		{
			FVector TargetLocation = TargetRouteNode->GetActorLocation();
			BlackboardComp->SetValueAsVector(TargetNode.SelectedKeyName, TargetLocation);
			return EBTNodeResult::Succeeded;
		}
		
		return EBTNodeResult::Failed;
	}
	else
	{
		for (int32 i = OriginalIndex; i < SafeNodes.Num() - 1; ++i)
		{
			AActor* StartNode = SafeNodes[i];
			AActor* EndNode = SafeNodes[i+1];
        
			if (!IsValid(StartNode) || !IsValid(EndNode)) continue;
        
			const FVector StartLoc = StartNode->GetActorLocation();
			const FVector EndLoc = EndNode->GetActorLocation();
			const FVector ClosestPoint = GetClosestPointOnSegment(CurrentLocation, StartLoc, EndLoc);
        
			const float CurrentDist = FVector::Distance(CurrentLocation, ClosestPoint);
			if (CurrentDist < MinDistance)
			{
				MinDistance = CurrentDist;
				BestSegmentEndIndex = i + 1;
			}
		}

		if (BestSegmentEndIndex < SafeNodes.Num() && IsValid(SafeNodes[BestSegmentEndIndex]))
		{
			AActor* TargetRouteNode = SafeNodes[BestSegmentEndIndex];

			FVector TargetLocation = TargetRouteNode->GetActorLocation();

			BlackboardComp->SetValueAsInt(TargetNodeIndex.SelectedKeyName, BestSegmentEndIndex);
			BlackboardComp->SetValueAsVector(TargetNode.SelectedKeyName, TargetLocation);
		}
		else
		{
			BlackboardComp->SetValueAsInt(TargetNodeIndex.SelectedKeyName, OriginalIndex);
		}
	}
	return EBTNodeResult::Succeeded;
	
}

FVector UPVZ3DTaskSetTargetNodeLocation::GetClosestPointOnSegment(const FVector& P, const FVector& A,
	const FVector& B) const
{
	const FVector AB = B - A;
	const float MagnitudeAB = AB.SizeSquared();
	if (MagnitudeAB < KINDA_SMALL_NUMBER) return A;

	const FVector AP = P - A;
	const float t = FMath::Clamp(FVector::DotProduct(AP, AB) / MagnitudeAB, 0.0f, 1.0f);
	return A + t * AB;
}