// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PVZ3DTowerPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DTowerPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetLastSeenTarget();
	AActor* SunFlowerGetLastSeenTarget();

	UPVZ3DTowerPerceptionComponent();
	
	bool HasValidTarget;

	AActor* BestTarget;
	
};
