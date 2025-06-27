// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "PVZ3DInventoryCellWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FReceived ,FName, int32 , int32)

DECLARE_MULTICAST_DELEGATE(FBrushWhite)

UCLASS()
class PVZ3D_API UPVZ3DInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FReceived Received;
	
	FBrushWhite BrushWhite;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* Datatable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	int32 Quantity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "SlotIndex")
	int32 SlotIndex = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UBorder* OuterBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" ,  meta = (BindWidget))
	UButton* Button_0;
	
	void UpdateInventoryCellWidget(FName P_ID , int32 P_Quantity , int32 P_SlotIndex);

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed();

protected:
	virtual void NativeOnInitialized() override;
};
