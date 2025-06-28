// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DPlayerHUD.h"
#include "Engine/Canvas.h"
#include "../UI/PVZ3DShopWidget.h"
#include "../UI/PVZ3DInventoryMainWidget.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogHUD,All,All);

void APVZ3DPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrossHair();
}

void APVZ3DPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerDetailWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerDetailWidgetClass);
	
	auto LevelWidget = CreateWidget<UUserWidget>(GetWorld(), LevelWidgetClass);

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

	if(InventoryInformationWidget)
	{
		InventoryInformationWidget->AddToViewport();
	}

	if(ShopWidget)
	{
		ShopWidget->AddToViewport();

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
            InventoryMainWidgetInstance->Received.AddUObject(this, &APVZ3DPlayerHUD::ReceivedInfo);
        }
	}
}

void APVZ3DPlayerHUD::ReceivedInfo(FName P_ID, int32 P_Quantity, int32 P_SlotIndex)
{
	if(InventoryInformationWidget && InventoryInformationWidget->GetClass()->ImplementsInterface(UPVZ3DShowInfoInterface::StaticClass()))
	{
			if (IPVZ3DShowInfoInterface* ShowInfoInterface = Cast<IPVZ3DShowInfoInterface>(InventoryInformationWidget))
			{
				ShowInfoInterface->ShowInfo(P_ID, P_Quantity);
			}
	}

	OnHoledSlotChanged.Broadcast(P_SlotIndex);
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
		}
		else
		{
			InventoryInformationWidget->SetVisibility(ESlateVisibility::Visible);
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
		}
		else
		{
			ShopWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
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




