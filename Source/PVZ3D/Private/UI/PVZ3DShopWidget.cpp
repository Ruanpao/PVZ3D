// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DShopWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "UI/PVZ3DShopCellWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogShop,All,All)

void UPVZ3DShopWidget::NativePreConstruct()
{
	Super::NativeConstruct();

	if(Button_1)
	{
		Button_1->OnClicked.AddDynamic(this , &UPVZ3DShopWidget::OnButton_1Clicked);
	}

	if(Button_2)
	{
		Button_2->OnClicked.AddDynamic(this , &UPVZ3DShopWidget::OnButton_2Clicked);
	}
	
	
	PlantsDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ShopPlantDataTable.DT_ShopPlantDataTable'"));
	ItemsDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ShopItemDataTable.DT_ShopItemDataTable'"));

	FName LevelName = "11";
	
	if(PlantsDataTable)
	{
		if (FShopStack* FoundPlantsStack = PlantsDataTable->FindRow<FShopStack>(LevelName , ""))
		{
			
			TArray<FName> PlantsStack = { FoundPlantsStack->Stack01, FoundPlantsStack->Stack02, FoundPlantsStack->Stack03, FoundPlantsStack->Stack04,FoundPlantsStack->Stack05, FoundPlantsStack->Stack06, FoundPlantsStack->Stack07, FoundPlantsStack->Stack08 };

			for(const FName& PlantID : PlantsStack)
			{
				if (UPVZ3DShopCellWidget* ShopCellWidget = CreateWidget<UPVZ3DShopCellWidget>(GetWorld(), ShopCellWidgetClass))
				{
					ShopCellWidget->UpdateShopCellWidget(PlantID);

					ShopCellWidget->ReceivedInfo.AddUObject(this, &UPVZ3DShopWidget::ReceivedInfo);

					ShopCellWidget->BrushWhite.AddUObject(this, &UPVZ3DShopWidget::BrushWhite);
					
					Grid_1->AddChildToWrapBox(ShopCellWidget);
				}
			}
		}
	}

	if(ItemsDataTable)
	{
		if (FShopStack* FoundItemsStack = ItemsDataTable->FindRow<FShopStack>(LevelName , ""))
		{
			TArray<FName> ItemsStack = { FoundItemsStack->Stack01, FoundItemsStack->Stack02, FoundItemsStack->Stack03, FoundItemsStack->Stack04,FoundItemsStack->Stack05, FoundItemsStack->Stack06, FoundItemsStack->Stack07, FoundItemsStack->Stack08 };

			for(const FName& ItemID : ItemsStack)
			{
				if (UPVZ3DShopCellWidget* ShopCellWidget = CreateWidget<UPVZ3DShopCellWidget>(GetWorld(), ShopCellWidgetClass))
				{
					ShopCellWidget->UpdateShopCellWidget(ItemID);

					ShopCellWidget->ReceivedInfo.AddUObject(this, &UPVZ3DShopWidget::ReceivedInfo);

					ShopCellWidget->BrushWhite.AddUObject(this, &UPVZ3DShopWidget::BrushWhite);
					
					Grid_2->AddChildToWrapBox(ShopCellWidget);
				}
			}
		}
	}
}

void UPVZ3DShopWidget::UpdateShopWidget()
{
	Grid_1->ClearChildren();
	Grid_2->ClearChildren();
	NativePreConstruct();
}

void UPVZ3DShopWidget::ReceivedInfo(FName ID,int32 Price)
{
	ReceivedInfo_2.Broadcast(ID , 1 , Price);
}

void UPVZ3DShopWidget::OnButton_1Clicked()
{
	if(WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UPVZ3DShopWidget::OnButton_2Clicked()
{
	if(WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UPVZ3DShopWidget::BrushWhite()
{
	for (auto& Child : Grid_1->GetAllChildren())
	{
		if(UPVZ3DShopCellWidget* ShopCellWidget = Cast<UPVZ3DShopCellWidget>(Child))
		{
			if(ShopCellWidget->OuterBorder)
			{
				ShopCellWidget->OuterBorder->SetBrushColor(FLinearColor::White);
			}
		}
	}

	for (auto& Child : Grid_2->GetAllChildren())
	{
		if(UPVZ3DShopCellWidget* ShopCellWidget = Cast<UPVZ3DShopCellWidget>(Child))
		{
			if(ShopCellWidget->OuterBorder)
			{
				ShopCellWidget->OuterBorder->SetBrushColor(FLinearColor::White);
			}
		}
	}
}

