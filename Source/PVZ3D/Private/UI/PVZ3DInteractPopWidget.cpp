// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PVZ3DInteractPopWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "PVZ3D/CoreTypes/PVZ3DWeaponCoreTypes.h"

void UPVZ3DInteractPopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SendHoldedItem.AddUObject(this, &UPVZ3DInteractPopWidget::UpdateHoldedItem);

	if(!DataTable)
	{
		DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_HoldedItemDataTable.DT_HoldedItemDataTable'"));
	}
	
	if(Button_Construct)
	{
		Button_Construct->OnClicked.AddDynamic(this, &UPVZ3DInteractPopWidget::OnButton_ConstructClicked);
	}

	if(Button_Upgrade)
	{
		Button_Upgrade->OnClicked.AddDynamic(this, &UPVZ3DInteractPopWidget::OnButton_UpgradeClicked);
	}

	if(Button_Sell)
	{
		Button_Sell->OnClicked.AddDynamic(this, &UPVZ3DInteractPopWidget::OnButton_SellClicked);
	}

	if(Button_TakeInHand)
	{
		Button_TakeInHand->OnClicked.AddDynamic(this, &UPVZ3DInteractPopWidget::OnButton_TakeInHandClicked);
	}

	if(Button_Construct && Button_Upgrade && Button_Sell && Button_TakeInHand)
	{
		Button_Construct->SetVisibility(ESlateVisibility::Hidden);
		Button_Upgrade->SetVisibility(ESlateVisibility::Hidden);
		Button_Sell->SetVisibility(ESlateVisibility::Hidden);
		Button_TakeInHand->SetVisibility(ESlateVisibility::Hidden);
	}
	
	
	WhetherClickedAndNearTower.AddUObject(this, &UPVZ3DInteractPopWidget::RealOnInitialized);
	
}

void UPVZ3DInteractPopWidget::UpdateHoldedItem(FItemInInventory NewHoldedItem)
{
	HoldedItem = NewHoldedItem;
}

void UPVZ3DInteractPopWidget::RealOnInitialized(bool IsClicked , bool IsNearTower, bool IsFullTower , bool IsFullLevel , bool IsIntheMindLine)
{
	if(IsClicked)
	{
		if(IsFullTower)
		{
			if(!IsFullLevel)
			{
				Button_Upgrade->SetVisibility(ESlateVisibility::Visible);
			}
			Button_Sell->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{ 	
			if(FWeaponBasicInfo* FoundWeaponInfo = DataTable->FindRow<FWeaponBasicInfo>(HoldedItem.ID, ""))
			{
				if(FoundWeaponInfo->ItemType == "Plant_Attack" && !IsIntheMindLine)
				{
					Button_Construct->SetVisibility(ESlateVisibility::Visible);
				}
				else if(FoundWeaponInfo->ItemType == "Plant_Defense" && IsIntheMindLine)
				{
					Button_Construct->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}

		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		if(CanvasPanel && VerticalBox)
		{
			UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(VerticalBox->Slot);
			if(CanvasPanelSlot)
			{
				CanvasPanelSlot->SetPosition(MousePosition);
			
				CanvasPanelSlot->SetZOrder(1000); // Ensure it's on top
			}
		}
		
	}
	else if(!IsClicked && IsNearTower)
	{
		if(IsFullTower)
		{
			if(!IsFullLevel)
			{
				Button_Upgrade->SetVisibility(ESlateVisibility::Visible);
			}
			Button_Sell->SetVisibility(ESlateVisibility::Visible);

			Button_TakeInHand->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			if(FWeaponBasicInfo* FoundWeaponInfo = DataTable->FindRow<FWeaponBasicInfo>(HoldedItem.ID, ""))
			{
				if(FoundWeaponInfo->ItemType == "Plant_Attack" && !IsIntheMindLine)
				{
					Button_Construct->SetVisibility(ESlateVisibility::Visible);
				}
				else if(FoundWeaponInfo->ItemType == "Plant_Defense" && IsIntheMindLine)
				{
					Button_Construct->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void UPVZ3DInteractPopWidget::OnButton_ConstructClicked()
{
	
}

void UPVZ3DInteractPopWidget::OnButton_UpgradeClicked()
{
	
}

void UPVZ3DInteractPopWidget::OnButton_SellClicked()
{
	
}

void UPVZ3DInteractPopWidget::OnButton_TakeInHandClicked()
{
	
}

void UPVZ3DInteractPopWidget::OnMouseLeave_1()
{
	this->RemoveFromParent();
}