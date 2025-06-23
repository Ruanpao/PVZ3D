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

				for(auto& Item : InventoryComponent->Slot)
				{
					if(UPVZ3DInventoryCellWidget* InventoryCellWidget = CreateWidget<UPVZ3DInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
					{
						InventoryCellWidget->UpdateInventoryCellWidget(Item.ID, Item.Quantity);

						InventoryCellWidget->Received.AddUObject(this, &UPVZ3DInventoryMainWidget::ReceivedInfo);

						InventoryCellWidget->BrushWhite.AddUObject(this, &UPVZ3DInventoryMainWidget::BrushWhite);

						Grid->AddChildToWrapBox(InventoryCellWidget);
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

void UPVZ3DInventoryMainWidget::ReceivedInfo(FName P_ID, int32 P_Quantity)
{
	Received.Broadcast(P_ID, P_Quantity);
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




