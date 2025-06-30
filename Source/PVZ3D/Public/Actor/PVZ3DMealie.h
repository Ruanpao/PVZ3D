// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DItemBuff.h"
#include "PVZ3DMealie.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DMealie : public APVZ3DItemBuff
{
	GENERATED_BODY()

public:
	APVZ3DMealie();
	virtual void ApplyBuff(UPVZ3DHealthComponent* HealthComponent) override;
	virtual void StopUse() override;
protected:

	// 每次治疗量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal")
	float HealAmount = 100.0f;
	// 持续治疗时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal")
	float HealDuration = 5.0f;
};