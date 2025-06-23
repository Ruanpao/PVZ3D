// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTowerPerceptionComponent.h"
#include "AI/PVZ3DTowerController.h"
#include "PVZ3DHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UPVZ3DTowerPerceptionComponent::GetLastSeenTarget()
{
	if (!this)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPVZ3DTowerPerceptionComponent is null"));
		return nullptr;
	}

	TArray<AActor*> SeenActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);
	//UE_LOG(LogTemp, Warning, TEXT("SeenActors: %d"), SeenActors.Num());
	if (SeenActors.Num() == 0)
	{
		return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller)
	{
		return nullptr;
	}

	const auto Tower = Controller->GetPawn();
	if (!Tower)
	{
		return nullptr;
	}

	float BestDistance = MAX_FLT;
	AActor* BestTarget = nullptr;
	for (const auto SeenActor : SeenActors)
	{
		// 检查 SeenActor 是否为空
		if (SeenActor)
		{
			const auto CurrentDistance = (SeenActor->GetActorLocation() - Tower->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestTarget = SeenActor;
			}
		}
	}
	return BestTarget;
}
