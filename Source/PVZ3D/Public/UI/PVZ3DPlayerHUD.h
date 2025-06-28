// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "../Interface/PVZ3DShowInfoInterface.h"
#include "PVZ3DPlayerHUD.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FSendInfo, FName , int32)

DECLARE_MULTICAST_DELEGATE_ThreeParams(FBuy , FName, int32 , int32)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoledSlotChanged , int32)

DECLARE_MULTICAST_DELEGATE_ThreeParams(FRemoveItem , int32 , bool , bool)


UCLASS()
class PVZ3D_API APVZ3DPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	int32 RemoveIndex;
	
	virtual void DrawHUD() override;

	void InventoryInformationVisibility();

	void ShopVisibility();

	void RemoveRequest(int32 Index);

	void RemoveRequest_2(bool RemoveAll);

	FBuy Buy;
	
	FSendInfo SendInfo;

	FOnHoledSlotChanged OnHoledSlotChanged;

	FRemoveItem RemoveItem;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> PlayerDetailWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> LevelWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> InventoryMainWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> InventoryInformationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI")
	TSubclassOf<UUserWidget> DisposalPopWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category =  "UI")
	TSubclassOf<UUserWidget> HoldedItemWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* InventoryInformationWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* ShopWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* InventoryMainWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UIInstance")
	UUserWidget* DisposalPopWidget;
	
	UFUNCTION(BlueprintCallable)
	void ReceivedInfo(FName P_ID, int32 P_Quantity, int32 P_SlotIndex);

	UFUNCTION(BlueprintCallable)
	void ReceivedInfo_2(FName P_ID, int32 P_Quantity , int32 P_Price);

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};
