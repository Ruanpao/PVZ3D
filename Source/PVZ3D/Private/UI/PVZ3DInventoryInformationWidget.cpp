// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DInventoryInformationWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "../../CoreTypes/ItemCoreTypes.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogInformationWidget,All,All)

void UPVZ3DInventoryInformationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	if(GetWorld())
	{
		TArray<AActor*> PlayerTaggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
    
		// 遍历查找符合条件的Pawn
		for (AActor* Actor : PlayerTaggedActors)
		{
			if (APawn* PlayerPawn = Cast<APawn>(Actor))
			{
				if (UPVZ3DInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UPVZ3DInventoryComponent>())
				{
					InventoryComponent->HoldedChanged.AddUObject(this, &UPVZ3DInventoryInformationWidget::ShowInfo);
					break;
				}
			}
		}
	}
}

void UPVZ3DInventoryInformationWidget::ShowInfo(FItemInInventory HoldedItem)
{
	ID = HoldedItem.ID;
	Quantity = HoldedItem.Quantity;

	UE_LOG(LogTemp,Warning,TEXT("HHEELLOO"));

	if(!Datatable)
	{
		Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
	}

	if(Datatable)
	{
		if(ID == "0000")
		{
			Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));
			
			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Hidden);
				DescriptionBox->SetVisibility(ESlateVisibility::Hidden);
				InformationBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(ID , "");

			if(FoundItemInfo)
			{
				Icon = FoundItemInfo->Icon;
				Name = FText::FromName(FoundItemInfo->Name);
				MaxStackNum = FoundItemInfo->MaxStackNum;
				Description = FoundItemInfo->Description;
				FString Information1 = FoundItemInfo->Information.ToString();
				FString Information2 = Information1.Replace(TEXT("\\n"), TEXT("\n"));
				Information = FText::FromString(Information2);
				ItemType = FText::FromName(FoundItemInfo->ItemType);
			}

			if(DetailBox && DescriptionBox && InformationBox)
			{
				DetailBox->SetVisibility(ESlateVisibility::Visible);
				DescriptionBox->SetVisibility(ESlateVisibility::Visible);
				InformationBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

