// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "UI/PVZ3DPlayerHUD.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "PVZ3DPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API APVZ3DPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	APVZ3DPlayerController();

	virtual void SetupInputComponent() override;

	void ToggleInventoryInformationVisibility();

	void ToggleShopVisibility();
	
protected:
	APVZ3DPlayerHUD* MyHUD;

	APVZ3DPlayer* MyPlayer;

	virtual void BeginPlay() override;

	void InitializeHUD();
};
