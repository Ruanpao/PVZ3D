// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/PVZ3DItem.h"
#include "Component/PVZ3DHealthComponent.h"
#include "PVZ3DItemBuff.generated.h"


/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DItemBuff : public APVZ3DItem
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Buff")
	virtual void ApplyBuff(UPVZ3DHealthComponent* HealthComponent);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Buff")
	float BuffValue = 0.0f;
};