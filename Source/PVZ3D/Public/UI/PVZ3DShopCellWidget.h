// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "PVZ3DShopCellWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FRecivedinfo , FName , int32)

DECLARE_MULTICAST_DELEGATE(FBrushWhite)

UCLASS()
class PVZ3D_API UPVZ3DShopCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FRecivedinfo ReceivedInfo;

	FBrushWhite BrushWhite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* Datatable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "ItemInfo")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" , meta = (BindWidget))
	UBorder* OuterBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" ,  meta = (BindWidget))
	UButton* Button_0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "CellUI" ,  meta = (BindWidget))
	int32 Price;

	void UpdateShopCellWidget(FName P_ID);

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed();

protected:
	virtual void NativeOnInitialized() override;
};

