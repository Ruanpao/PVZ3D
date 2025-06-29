// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "PVZ3DInventoryCellWidget.h"
#include "PVZ3DInventoryMainWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FReceived_2 ,int32)

DECLARE_MULTICAST_DELEGATE_OneParam(FReceivedRemove ,  int32)

UCLASS()
class PVZ3D_API UPVZ3DInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FReceived_2 Received_2;

	FReceivedRemove ReceivedRemove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI")
	TSubclassOf<UPVZ3DInventoryCellWidget> InventoryCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UWrapBox* Grid;
	
	UFUNCTION(BlueprintCallable)
	void ReceivedInfo(int32 P_SlotIndex);

	UFUNCTION(BlueprintCallable)
	void BrushWhite();

	UFUNCTION(BlueprintCallable)
	void ReceivedRemoveInfo(int32 P_SlotIndex);

protected:
	virtual void NativePreConstruct() override;

	void UpdateMainWidget();
};
