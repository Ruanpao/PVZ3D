// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PVZ3DLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DLevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	int32 CurrentWave = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	int32 TotalWaves = 0;

	UPROPERTY(EditDefaultsOnly,	BlueprintReadWrite, Category = "UI")
	int32 RemainingEnemy = 0;

	UPROPERTY(EditDefaultsOnly,	BlueprintReadWrite, Category = "UI")
	float RemainingTime = 0.0f;
	
protected:
	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentWave(int32 NewCurrentWave);

	UFUNCTION(BlueprintCallable)
	void UpdateRemainingEnemy(int32 NewRemainingEnemies);

	UFUNCTION(BlueprintCallable)
	void UpdateTotalWaves(int32 NewTotalWaves);
};
