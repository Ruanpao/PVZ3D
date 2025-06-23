// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTowerPerceptionComponent.h"
#include "AI/PVZ3DTowerController.h"
#include "PVZ3DHealthComponent.h"
#include "VectorTypes.h"
#include "Perception/AISense_Sight.h"
#include "AI/PVZ3DEnemy.h"
#include "AI/PVZ3DTower.h"

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

AActor* UPVZ3DTowerPerceptionComponent::SunFlowerGetLastSeenTarget()
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
		HasValidTarget=0;
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

	if(BestTarget)
	{
		if(Cast<APVZ3DEnemy>(BestTarget)->IsDead || (BestTarget->GetActorLocation()-Tower->GetActorLocation()).Size() > Cast<APVZ3DTower>(Tower)->AttackRange)
		{
			BestTarget = nullptr;
			HasValidTarget=0;
		}
	}
	
	for (const auto SeenActor : SeenActors)
	{
		// 检查 SeenActor 是否为空
		if (SeenActor && !HasValidTarget)
		{
			BestTarget = SeenActor;
			HasValidTarget = 1;
			UE_LOG(LogTemp, Warning, TEXT("SeenActor: %s"), *SeenActor->GetName());
		}
		
	}
	return BestTarget;
}

UPVZ3DTowerPerceptionComponent::UPVZ3DTowerPerceptionComponent()
{
	HasValidTarget=0;
	BestTarget = nullptr;
}
