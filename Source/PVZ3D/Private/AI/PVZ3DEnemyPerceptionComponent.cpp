// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemyPerceptionComponent.h"
#include "AI/PVZ3DEnemyController.h"
#include "PVZ3DHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UPVZ3DEnemyPerceptionComponent::GetLastSeenTarget()
{
	// 检查 this 指针是否为空
	if (!this)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPVZ3DEnemyPerceptionComponent is null"));
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

	const auto Enemy = Controller->GetPawn();
	if (!Enemy)
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
			const auto CurrentDistance = (SeenActor->GetActorLocation() - Enemy->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestTarget = SeenActor;
			}
		}
	}
	return BestTarget;
}
