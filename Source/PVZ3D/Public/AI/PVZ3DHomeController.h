// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PVZ3DHomeController.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DHomeController : public AAIController
{
	GENERATED_BODY()

public:
	APVZ3DHomeController();

private:
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
};
