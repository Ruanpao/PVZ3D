// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryMainWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "UI/PVZ3DInventoryCellWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMainWidget,All,All);

void UPVZ3DInventoryMainWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if(GetWorld())
	{
		if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if(UPVZ3DInventoryComponent* InventoryComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DInventoryComponent>())
			{
				InventoryComponent->OnInventoryUpdate.AddUObject(this, &UPVZ3DInventoryMainWidget::UpdateMainWidget);

				int32 Index = 0;
				for(auto& Item : InventoryComponent->Slot)
				{
					if(UPVZ3DInventoryCellWidget* InventoryCellWidget = CreateWidget<UPVZ3DInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
					{
						InventoryCellWidget->UpdateInventoryCellWidget(Item.ID, Item.Quantity , Index);

						InventoryCellWidget->Received.AddUObject(this, &UPVZ3DInventoryMainWidget::ReceivedInfo);

						InventoryCellWidget->BrushWhite.AddUObject(this, &UPVZ3DInventoryMainWidget::BrushWhite);

						InventoryCellWidget->Remove.AddUObject(this, )

						Grid->AddChildToWrapBox(InventoryCellWidget);

						Index += 1;
					}
				}
			}
		}
	}
}

void UPVZ3DInventoryMainWidget::UpdateMainWidget()
{
	Grid->ClearChildren();
	NativePreConstruct();
}

void UPVZ3DInventoryMainWidget::ReceivedInfo(FName P_ID, int32 P_Quantity , int32 P_SlotIndex)
{
	Received.Broadcast(P_ID, P_Quantity , P_SlotIndex);
}

void UPVZ3DInventoryMainWidget::BrushWhite()
{
	for (auto& Child : Grid->GetAllChildren())
	{
		if(UPVZ3DInventoryCellWidget* InventoryCellWidget = Cast<UPVZ3DInventoryCellWidget>(Child))
		{
			if(InventoryCellWidget->OuterBorder)
			{
				InventoryCellWidget->OuterBorder->SetBrushColor(FLinearColor::White);
			}
		}
	}
}

void UPVZ3DInventoryMainWidget::ReceivedRemoveInfo(int32 P_SlotIndex)
{
	ReceivedRemove.Broadcast(P_SlotIndex);
}





