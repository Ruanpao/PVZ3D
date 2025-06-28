// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryCellWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogCellWidget,All,All)

void UPVZ3DInventoryCellWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	if(Button_0)
	{
		Button_0->OnClicked.AddDynamic(this , &UPVZ3DInventoryCellWidget::OnButtonClicked);

		Button_0->OnPressed.AddDynamic(this , &UPVZ3DInventoryCellWidget::OnButtonPressed);
	}
}

void UPVZ3DInventoryCellWidget::UpdateInventoryCellWidget(FName P_ID , int32 P_Quantity , int32 P_SlotIndex)
{
	ID = P_ID;
	Quantity = P_Quantity;
	SlotIndex = P_SlotIndex;

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
	}
	
	if(Datatable)
	{
		if (FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , ""))
		{
			Icon = FoundItemInfo->Icon;
		}
	}
}

void UPVZ3DInventoryCellWidget::OnButtonClicked()
{
	Received.Broadcast(ID, Quantity , SlotIndex);

	BrushWhite.Broadcast();
	
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Green);
	}
}

void UPVZ3DInventoryCellWidget::OnButtonPressed()
{
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Yellow);
	}
}


