// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PVZ3DShopCellWidget.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "PVZ3DShopWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FReceivedInfo_2, FName , int32 , int32)

UCLASS()
class PVZ3D_API UPVZ3DShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FReceivedInfo_2 ReceivedInfo_2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* PlantsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* ItemsDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI")
	TSubclassOf<UPVZ3DShopCellWidget> ShopCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UWrapBox* Grid_1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UWrapBox* Grid_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" ,  meta = (BindWidget))
	UButton* Button_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UButton* Button_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UFUNCTION(BlueprintCallable)
	void OnButton_1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_2Clicked();

	UFUNCTION(BlueprintCallable)
	void BrushWhite();
	

protected:
	virtual void NativePreConstruct() override;

	void UpdateShopWidget();

	void ReceivedInfo(FName ID,int32 Price);
};
