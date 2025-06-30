// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryMainWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "UI/PVZ3DInventoryCellWidget.h"
#include "../../CoreTypes/ItemCoreTypes.h"
#include "UI/PVZ3DInventoryCellWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMainWidget,All,All);


void UPVZ3DInventoryMainWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (GetWorld())
    {
        TArray<AActor*> PlayerTaggedActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
        
        for (AActor* Actor : PlayerTaggedActors)
        {
            if (APawn* PlayerPawn = Cast<APawn>(Actor))
            {
                if (UPVZ3DInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UPVZ3DInventoryComponent>())
                {
                    // 仅在未绑定时执行绑定
                    if (!bIsDelegateBound)
                    {
                        InventoryComponent->OnInventoryUpdate.AddUObject(this, &UPVZ3DInventoryMainWidget::UpdateMainWidget);
                        bIsDelegateBound = true;
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate bound successfully"));
                    }
                    else
                    {
                        UE_LOG(LogMainWidget, Log, TEXT("Delegate already bound, skipping"));
                    }
                    
                    int32 Index = 0;
                    for (auto& Item : InventoryComponent->Slot)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Nakamura01 ID: %s , Quantity: %d , Index: %d, Name:%s"), 
                               *Item.ID.ToString(), Item.Quantity, Index, *GetName());
                        
                        if (UPVZ3DInventoryCellWidget* InventoryCellWidget = 
                            CreateWidget<UPVZ3DInventoryCellWidget>(GetWorld(), InventoryCellWidgetClass))
                        {
                            InventoryCellWidget->UpdateInventoryCellWidget(Item.ID, Item.Quantity, Index);
                            InventoryCellWidget->Received_1.AddUObject(this, &UPVZ3DInventoryMainWidget::ReceivedInfo);
                            InventoryCellWidget->BrushWhite.AddUObject(this, &UPVZ3DInventoryMainWidget::BrushWhite);
                            InventoryCellWidget->Remove.AddUObject(this, &UPVZ3DInventoryMainWidget::ReceivedRemoveInfo);
                            Grid->AddChildToWrapBox(InventoryCellWidget);
                            Index += 1;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void UPVZ3DInventoryMainWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UPVZ3DInventoryMainWidget::UpdateMainWidget()
{
    UE_LOG(LogMainWidget, Log, TEXT("Updating main widget"));
    Grid->ClearChildren();
    NativePreConstruct(); // 保留原有逻辑，但委托不会重复绑定
}

// 添加析构函数解除委托绑定
UPVZ3DInventoryMainWidget::~UPVZ3DInventoryMainWidget()
{
    if (bIsDelegateBound)
    {
        UE_LOG(LogMainWidget, Log, TEXT("Destroying widget, unbinding delegate"));
        bIsDelegateBound = false;
    }
}

// 其他函数保持不变
void UPVZ3DInventoryMainWidget::ReceivedInfo(int32 P_SlotIndex)
{
    Received_2.Broadcast(P_SlotIndex);
}

void UPVZ3DInventoryMainWidget::BrushWhite()
{
    for (auto& Child : Grid->GetAllChildren())
    {
        if (UPVZ3DInventoryCellWidget* InventoryCellWidget = Cast<UPVZ3DInventoryCellWidget>(Child))
        {
            if (InventoryCellWidget->OuterBorder)
            {
                InventoryCellWidget->OuterBorder->SetBrushColor(FLinearColor::White);
            }
        }
    }
}

void UPVZ3DInventoryMainWidget::ReceivedRemoveInfo(int32 P_SlotIndex)
{
    ReceivedRemove.Broadcast(P_SlotIndex);
    UE_LOG(LogMainWidget, Warning, TEXT("OnMouseButtonDown_Right SlotIndex: %d"), P_SlotIndex);
}