// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DDisposalPopWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UPVZ3DDisposalPopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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
	

	if(Button_0)
	{
		Button_0->OnClicked.AddDynamic(this , &UPVZ3DDisposalPopWidget::OnButton_0Clicked);
	}

	if(Button_1)
	{
		Button_1->OnClicked.AddDynamic(this , &UPVZ3DDisposalPopWidget::OnButton_1Clicked);
	}
}

void UPVZ3DDisposalPopWidget::OnButton_0Clicked()
{
	Remove_2.Broadcast(false);

	this->RemoveFromParent();
}

void UPVZ3DDisposalPopWidget::OnButton_1Clicked()
{
	Remove_2.Broadcast(true);

	this->RemoveFromParent();
}

void UPVZ3DDisposalPopWidget::OnMouseLeave_1()
{
	this->RemoveFromParent();
}


