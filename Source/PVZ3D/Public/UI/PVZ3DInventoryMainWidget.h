// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "PVZ3DInventoryCellWidget.h"
#include "PVZ3DInventoryMainWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FReceived , FName , int32)

UCLASS()
class PVZ3D_API UPVZ3DInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FReceived Received;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI")
	TSubclassOf<UPVZ3DInventoryCellWidget> InventoryCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UWrapBox* Grid;
	
	UFUNCTION(BlueprintCallable)
	void ReceivedInfo(FName P_ID, int32 P_Quantity);

	UFUNCTION(Blueprintcallable)
	void BrushWhite();

protected:
	virtual void NativePreConstruct() override;

	void UpdateMainWidget();
};
