// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "../Interface/PVZ3DShowInfoInterface.h"
#include "PVZ3DPlayerHUD.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FSendInfo, FName , int32)

UCLASS()
class PVZ3D_API APVZ3DPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	FSendInfo SendInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> PlayerDetailWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> LevelWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> InventoryMainWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> InventoryInformationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* InventoryInformationWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* InventoryMainWidget;

	void DisplayInventoryInformationWidget();

	void RemoveInventoryInformationWidget();

	UFUNCTION(BlueprintCallable)
	void ReceivedInfo(FName P_ID, int32 P_Quantity);

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};
