// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DPlayerHUD.h"
#include "Engine/Canvas.h"
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

	InventoryMainWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryMainWidgetClass);
	
	if(PlayerDetailWidget)
	{
		PlayerDetailWidget->AddToViewport();
	}

	if(LevelWidget)
	{
		LevelWidget->AddToViewport();
	}

	if(InventoryMainWidget)
	{
		InventoryMainWidget->AddToViewport();

		if (UPVZ3DInventoryMainWidget* InventoryMainWidgetInstance = Cast<UPVZ3DInventoryMainWidget>(InventoryMainWidget))
        {
            InventoryMainWidgetInstance->Received.AddUObject(this, &APVZ3DPlayerHUD::ReceivedInfo);
        }
	}


	
	
	InventoryInformationWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryInformationWidgetClass);
	
	if(InventoryInformationWidget)
	{
		InventoryInformationWidget->AddToViewport();
	}



	
}

void APVZ3DPlayerHUD::DisplayInventoryInformationWidget()
{
	InventoryInformationWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryInformationWidgetClass);
	
	if(InventoryInformationWidget)
	{
		InventoryInformationWidget->AddToViewport();
	}
}

void APVZ3DPlayerHUD::RemoveInventoryInformationWidget()
{
	if(InventoryInformationWidget)
	{
		InventoryInformationWidget->RemoveFromParent();
	}
}

void APVZ3DPlayerHUD::ReceivedInfo(FName P_ID, int32 P_Quantity)
{
	if(InventoryInformationWidget && InventoryInformationWidget->GetClass()->ImplementsInterface(UPVZ3DShowInfoInterface::StaticClass()))
	{
			if (IPVZ3DShowInfoInterface* ShowInfoInterface = Cast<IPVZ3DShowInfoInterface>(InventoryInformationWidget))
			{
				ShowInfoInterface->ShowInfo(P_ID, P_Quantity);
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





