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

	if(Button_Remove)
	{
		Button_Remove->OnClicked.AddDynamic(this, &UPVZ3DInteractPopWidget::OnButton_RemoveClicked);
	}
	if(Button_Construct && Button_Upgrade && Button_Sell && Button_TakeInHand && Button_Remove)
	{
		Button_Construct->SetVisibility(ESlateVisibility::Hidden);
		Button_Upgrade->SetVisibility(ESlateVisibility::Hidden);
		Button_Sell->SetVisibility(ESlateVisibility::Hidden);
		Button_TakeInHand->SetVisibility(ESlateVisibility::Hidden);
		Button_Remove->SetVisibility(ESlateVisibility::Visible);
	}
	
	
	WhetherClickedAndNearTower.AddUObject(this, &UPVZ3DInteractPopWidget::RealOnInitialized);

	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerActors);

	UE_LOG(LogTemp,Error,TEXT("WidgetName : %s"), *GetName());
	UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK Tryget by Tag"));
	

	if (PlayerActors.Num() > 0)
	{
		AActor* PlayerActor = PlayerActors[0];
		UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK: %s"), *PlayerActor->GetName());
    
		APVZ3DPlayer* Player = Cast<APVZ3DPlayer>(PlayerActor);
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS Getplayer: %s"), *Player->GetName());
			Tower = Player->InteractingTower;
        
			if (Tower)
			{
				UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS InteractingTower: %s"), *Tower->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS InteractingTower is nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS Failed to cast to APVZ3DPlayer"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS No actors with 'Player' tag found"));
	}
	
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
	UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK: %s"),(Tower ? *Tower->GetName() : TEXT("None")));
	if(Tower)
	{
		Tower->SwapWeaponsWithPlayer(Tower->Player);
	}
	this->RemoveFromParent();
}

void UPVZ3DInteractPopWidget::OnButton_UpgradeClicked()
{
	if(Tower)
	{
		Tower->BuildTower(Tower->NextTowerID);
	}
	this->RemoveFromParent();

}

void UPVZ3DInteractPopWidget::OnButton_SellClicked()
{
	this->RemoveFromParent();

}

void UPVZ3DInteractPopWidget::OnButton_TakeInHandClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("HOLYJESUS HUDCLICK: %s"),(Tower ? *Tower->GetName() : TEXT("None")));
	if(Tower)
	{
		Tower->TakeInHandTower();
	}
	this->RemoveFromParent();

}

void UPVZ3DInteractPopWidget::OnButton_RemoveClicked()
{
	UE_LOG(LogTemp ,Error , TEXT("123"));
	this->RemoveFromParent();
}

void UPVZ3DInteractPopWidget::OnTowerInteractionReceived(bool bTowerHasWeapon, bool bTowerBaseInMiddle, bool bTowerWeaponMaxLevel, bool bIsNearTower, APVZ3DTower* aTower)
{
	Tower=aTower;

	// 在这里处理接收到的塔信息
	UE_LOG(LogTemp, Warning, TEXT("Received tower info: bTowerHasWeapon: %d, bTowerBaseInMiddle: %d, bTowerWeaponMaxLevel: %d, bIsNearTower: %d, Tower: %s"),
		   bTowerHasWeapon, bTowerBaseInMiddle, bTowerWeaponMaxLevel, bIsNearTower, (Tower ? *Tower->GetName() : TEXT("None")));
	// 可以根据接收到的信息更新交互弹窗的显示
	RealOnInitialized(false, bIsNearTower, bTowerHasWeapon, bTowerWeaponMaxLevel, bTowerBaseInMiddle);
}