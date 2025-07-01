// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DPlayerHUD.h"
#include "Engine/Canvas.h"
#include "../UI/PVZ3DShopWidget.h"
#include "../UI/PVZ3DInventoryMainWidget.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/PVZ3DDisposalPopWidget.h"

#include "Component/PVZ3DWeaponComponent.h"
#include "Character/PVZ3DPlayerController.h"
#include "Character/PVZ3DPlayer.h"
#include "UI/PVZ3DInteractPopWidget.h"


DEFINE_LOG_CATEGORY_STATIC(LogHUD, All, All);

void APVZ3DPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrossHair();
	
	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		if (APVZ3DPlayer* PlayerCharacter = Cast<APVZ3DPlayer>(PlayerController->GetPawn()))
		{
			if (UPVZ3DWeaponComponent* WeaponComponent = PlayerCharacter->FindComponentByClass<UPVZ3DWeaponComponent>())
			{
				WeaponComponent->ButtonInteraction.AddUObject(this, &APVZ3DPlayerHUD::CreateInteractPopWidget);
			}
		}
	}
}

void APVZ3DPlayerHUD::BeginPlay()
{
	Super::BeginPlay();



	
	
	auto PlayerDetailWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerDetailWidgetClass);
	
	auto LevelWidget = CreateWidget<UUserWidget>(GetWorld(), LevelWidgetClass);

	auto HoldedItemWidget = CreateWidget<UUserWidget>(GetWorld(), HoldedItemWidgetClass);

	ShopWidget = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);

	InventoryInformationWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryInformationWidgetClass);

	InventoryMainWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryMainWidgetClass);
	
	if(PlayerDetailWidget)
	{
		PlayerDetailWidget->AddToViewport();
	}

	if(LevelWidget)
	{
		LevelWidget->AddToViewport();
	}

	if(HoldedItemWidget)
	{
		HoldedItemWidget->AddToViewport();
	}

	if(InventoryInformationWidget)
	{
		InventoryInformationWidget->AddToViewport();
		
		InventoryInformationWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(ShopWidget)
	{
		ShopWidget->AddToViewport();

		ShopWidget->SetVisibility(ESlateVisibility::Hidden);

		if (UPVZ3DShopWidget* ShopWidgetInstance = Cast<UPVZ3DShopWidget>(ShopWidget))
		{
			ShopWidgetInstance->ReceivedInfo_2.AddUObject(this, &APVZ3DPlayerHUD::ReceivedInfo_2);
		}
		
	}
	
	if(InventoryMainWidget)
	{
		InventoryMainWidget->AddToViewport();

		if (UPVZ3DInventoryMainWidget* InventoryMainWidgetInstance = Cast<UPVZ3DInventoryMainWidget>(InventoryMainWidget))
        {
			InventoryMainWidgetInstance->Received_2.AddUObject(this, &APVZ3DPlayerHUD::ReceivedInfo);
			
			InventoryMainWidgetInstance->ReceivedRemove.AddUObject(this, &APVZ3DPlayerHUD::RemoveRequest);
        }
	}
}

void APVZ3DPlayerHUD::ReceivedInfo(int32 Index)
{
	OnHoledSlotChanged.Broadcast(Index);
}

void APVZ3DPlayerHUD::ReceivedInfo_2(FName P_ID, int32 P_Quantity , int32 P_Price)
{
	Buy.Broadcast(P_ID , P_Quantity , P_Price);
}

void APVZ3DPlayerHUD::InventoryInformationVisibility()
{
	if (InventoryInformationWidget)
	{
		if(InventoryInformationWidget->IsVisible())
		{
			InventoryInformationWidget->SetVisibility(ESlateVisibility::Hidden);
			UpdateMouseSituation(-1);
		}
		else
		{
			InventoryInformationWidget->SetVisibility(ESlateVisibility::Visible);
			UpdateMouseSituation(1);
		}
	}
}

void APVZ3DPlayerHUD::ShopVisibility()
{
	if(ShopWidget)
	{
		if(ShopWidget->IsVisible())
		{
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
			UpdateMouseSituation(-1);
		}
		else
		{
			ShopWidget->SetVisibility(ESlateVisibility::Visible);
			UpdateMouseSituation(1);
		}
	}
}

void APVZ3DPlayerHUD::RemoveRequest(int32 Index)
{
	RemoveIndex =  Index;

	DisposalPopWidget = CreateWidget<UUserWidget>(GetWorld(), DisposalPopWidgetClass);

	if(DisposalPopWidget)
	{
		DisposalPopWidget->AddToViewport();

		if(UPVZ3DDisposalPopWidget* DisposalPopWidgetInstance = Cast<UPVZ3DDisposalPopWidget>(DisposalPopWidget))
		{
			DisposalPopWidgetInstance->Remove_2.AddUObject(this, &APVZ3DPlayerHUD::RemoveRequest_2);
			DisposalPopWidgetInstance->NumChanged1.AddUObject(this, &APVZ3DPlayerHUD::UpdateMouseSituation);
		}
	}
	
}

void APVZ3DPlayerHUD::RemoveRequest_2(bool RemoveAll)
{
	RemoveItem.Broadcast(RemoveIndex , RemoveAll , false);

	UE_LOG(LogHUD , Warning , TEXT("RemoveIndex : %d , RemoveAll : %s"), RemoveIndex, RemoveAll ? TEXT("true") : TEXT("false"));
}

void APVZ3DPlayerHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX* 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max , Center.Min + HalfLineSize,Center.Max, LineColor,LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor,LineThickness);
}

void APVZ3DPlayerHUD::CreateInteractPopWidget(bool IsClicked, bool IsNearTower, bool IsFullTower , bool IsFullLevel , bool IsIntheMidLine , FItemInInventory HoldedItem)
{
	if (InteractPopWidget && InteractPopWidget->IsInViewport())
	{
		InteractPopWidget->RemoveFromParent();
		InteractPopWidget = nullptr;
	}
	
	InteractPopWidget = CreateWidget<UUserWidget>(GetWorld(), InteractPopWidgetClass);

	if(InteractPopWidget)
	{
		if(UPVZ3DInteractPopWidget* InteractPopWidgetInstance = Cast<UPVZ3DInteractPopWidget>(InteractPopWidget))
		{
			InteractPopWidgetInstance->SendHoldedItem.Broadcast(HoldedItem);
			InteractPopWidgetInstance->WhetherClickedAndNearTower.Broadcast(IsClicked, IsNearTower, IsFullTower, IsFullLevel, IsIntheMidLine);
			InteractPopWidgetInstance->OnNumChanged2.AddUObject(this, &APVZ3DPlayerHUD::UpdateMouseSituation);
		}

		InteractPopWidget->AddToViewport();
	}
}

void APVZ3DPlayerHUD::UpdateMouseSituation(int32 Num)
{
	CurrentVisibleNum += Num;

	UE_LOG(LogHUD ,  Error , TEXT("%d") , CurrentVisibleNum);
	
	if(CurrentVisibleNum > 0)
	{
		OnMouseSituationChanged.Broadcast(true);
	}
	else
	{
		OnMouseSituationChanged.Broadcast(false);
	}
}


