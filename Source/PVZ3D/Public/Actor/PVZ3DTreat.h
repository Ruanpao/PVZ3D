// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DItemBuff.h"
#include "PVZ3DTreat.generated.h"

UCLASS()
class PVZ3D_API APVZ3DTreat : public APVZ3DItemBuff
{
	GENERATED_BODY()

public:
	APVZ3DTreat();
    
	virtual void ApplyBuff(UPVZ3DHealthComponent* HealthComponent) override;
	virtual void StopUse() override;

protected:
	// 瞬间治疗量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal")
	float InstantHealAmount = 500.0f;

	// 治疗延迟时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal")
	float HealDelay = 0.5f;
};