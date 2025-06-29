// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "PVZ3DHoldedItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DHoldedItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	int32 Quantity = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FText Name = FText::FromString("None");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemInfo")
	FText ItemType = FText::FromString("Invalid ItemType");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FText CurrentBulletNum = FText::FromString("0");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FText MaxBulletNum = FText::FromString("0");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UHorizontalBox* BulletNumBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CellUI" , meta = (BindWidget))
	UHorizontalBox* ItemQuantityBox;

	void UpdateHoldedItemWidget(FItemInInventory HoldedItem);

protected:
	virtual void NativeOnInitialized() override;
};
