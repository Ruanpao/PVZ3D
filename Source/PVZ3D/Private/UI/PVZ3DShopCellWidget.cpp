// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DShopCellWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"

void UPVZ3DShopCellWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	if(Button_0)
	{
		Button_0->OnClicked.AddDynamic(this , &UPVZ3DShopCellWidget::OnButtonClicked);

		Button_0->OnPressed.AddDynamic(this , &UPVZ3DShopCellWidget::OnButtonPressed);
	}
}

void UPVZ3DShopCellWidget::UpdateShopCellWidget(FName P_ID)
{
	ID = P_ID;

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
	}

	if(Datatable)
	{
		if (FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , ""))
		{
			Icon = FoundItemInfo->Icon;
			Price = FoundItemInfo->Price;
		}
	}
}

void UPVZ3DShopCellWidget::OnButtonClicked()
{
	BrushWhite.Broadcast();
	
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Green);
	}

	ReceivedInfo.Broadcast(ID , Price);
}

void UPVZ3DShopCellWidget::OnButtonPressed()
{
	if(OuterBorder)
	{
		OuterBorder->SetBrushColor(FLinearColor::Yellow);
	}
}


