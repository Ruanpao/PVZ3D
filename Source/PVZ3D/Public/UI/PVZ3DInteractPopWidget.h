// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "Character/PVZ3DPlayer.h" 
#include "../../CoreTypes/ItemCoreTypes.h"
#include "PVZ3DInteractPopWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_FiveParams(FWhetherClickedAndNearTower, bool , bool , bool , bool , bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FSendHoldedItem, FItemInInventory)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNumChanged2, int32)

UCLASS()
class PVZ3D_API UPVZ3DInteractPopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FWhetherClickedAndNearTower WhetherClickedAndNearTower;

	FSendHoldedItem SendHoldedItem;

	FOnNumChanged2 OnNumChanged2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DataTable;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HoldedItem")
	FItemInInventory HoldedItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_Construct;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_Upgrade;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_Sell;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_TakeInHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_Remove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
	UFUNCTION(BlueprintCallable)
	void OnButton_ConstructClicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_UpgradeClicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_SellClicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_TakeInHandClicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_RemoveClicked();

	APVZ3DTower* Tower;

protected:
	virtual void NativeOnInitialized() override;

	void UpdateHoldedItem(FItemInInventory NewHoldedItem);

	void RealOnInitialized(bool IsClicked , bool IsNearTower , bool IsFullTower , bool IsFullLevel , bool IsIntheMidLine);
	UFUNCTION()
	void OnTowerInteractionReceived(bool bTowerHasWeapon, bool bTowerBaseInMiddle, bool bTowerWeaponMaxLevel, bool bIsNearTower, APVZ3DTower* aTower);
};
