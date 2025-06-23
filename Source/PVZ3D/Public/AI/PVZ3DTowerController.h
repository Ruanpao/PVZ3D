// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PVZ3DTowerController.generated.h"

/**
 * 
 */
class UPVZ3DTowerPerceptionComponent;

UCLASS()
class PVZ3D_API APVZ3DTowerController : public AAIController
{
	GENERATED_BODY()

public:
	APVZ3DTowerController();
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TArray<FGenericTeamId> FriendlyTeamID;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Team")
	TArray<FGenericTeamId> HostileTeamID;

	UPROPERTY(visibleAnywhere, Category = "Component")
	UPVZ3DTowerPerceptionComponent* TowerAIPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetKeyName = "Target";
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void UpdateTowerControllerImformation();

private:
	AActor* GetTargetActor() const;
};
