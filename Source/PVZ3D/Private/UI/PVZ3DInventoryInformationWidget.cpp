// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryInformationWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogInformationWidget,All,All)

void UPVZ3DInventoryInformationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));
}

void UPVZ3DInventoryInformationWidget::ShowInfo(FName P_ID, int32 P_Quantity)
{
	ID = P_ID;
	Quantity = P_Quantity;

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
	}

	if(Datatable)
	{
		if(ID == "0000")
		{
			Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));
			
			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Hidden);
				DescriptionBox->SetVisibility(ESlateVisibility::Hidden);
				InformationBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , "");

			if(FoundItemInfo)
			{
				Icon = FoundItemInfo->Icon;
				Name = FText::FromName(FoundItemInfo->Name);
				MaxStackNum = FoundItemInfo->MaxStackNum;
				Description = FoundItemInfo->Description;
				Information = FoundItemInfo->Information;
				ItemType = FoundItemInfo->ItemType;
			}

			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Visible);
				DescriptionBox->SetVisibility(ESlateVisibility::Visible);
				InformationBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

