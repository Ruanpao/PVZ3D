// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PVZ3DGamemode.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APVZ3DGamemode();
	
	float HomeHealth;
	
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void GameWin();

	UFUNCTION(BlueprintCallable,Category = "GameMode")
	void GameLose();

protected:
	
	
	
};
