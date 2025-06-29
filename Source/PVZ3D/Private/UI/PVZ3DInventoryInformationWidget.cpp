// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryInformationWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "../../CoreTypes/ItemCoreTypes.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInformationWidget,All,All)

void UPVZ3DInventoryInformationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	if(GetWorld())
	{
		if(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if(UPVZ3DInventoryComponent* InventoryComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DInventoryComponent>())
			{
				InventoryComponent->HoldedChanged.AddUObject(this, &UPVZ3DInventoryInformationWidget::ShowInfo);
			}
		}
	}
}

void UPVZ3DInventoryInformationWidget::ShowInfo(FItemInInventory HoldedItem)
{
	ID = HoldedItem.ID;
	Quantity = HoldedItem.Quantity;

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
				ItemType = FText::FromName(FoundItemInfo->ItemType);
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

