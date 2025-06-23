// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PVZ3DEnemyPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DEnemyPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetLastSeenTarget();
	
};
