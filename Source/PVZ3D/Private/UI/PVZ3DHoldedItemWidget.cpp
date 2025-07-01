// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DHoldedItemWidget.h"

#include "Component/PVZ3DWeaponComponent.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "../../CoreTypes/PVZ3DWeaponCoreTypes.h"
#include "Kismet/GameplayStatics.h"

void UPVZ3DHoldedItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Icon = LoadObject<UTexture2D>(this, TEXT("/Script/Engine.Texture2D'/Game/MyAsset/Texture/ItemNone.ItemNone'"));

	
	if(GetWorld())
	{
		TArray<AActor*> PlayerTaggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), PlayerTaggedActors);
		for (AActor* Actor : PlayerTaggedActors)
		{
			if (APawn* PlayerPawn = Cast<APawn>(Actor))
			{
				if (UPVZ3DInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<UPVZ3DInventoryComponent>())
				{
					InventoryComponent->HoldedChanged.AddUObject(this, &UPVZ3DHoldedItemWidget::UpdateHoldedItemWidget);
					break;
				}
			}

			if(UPVZ3DWeaponComponent* WeaponComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DWeaponComponent>())
			{
				WeaponComponent->Reloading.AddUObject(this, &UPVZ3DHoldedItemWidget::CurrentBulletNumChanged);
			}
		}
	}

	if(BulletNumBox && ItemQuantityBox)
	{
		BulletNumBox->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantityBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPVZ3DHoldedItemWidget::UpdateHoldedItemWidget(FItemInInventory HoldedItem)
{
	ID = HoldedItem.ID;
	Quantity = HoldedItem.Quantity;
	
	if(!DataTable)
	{
		DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_HoldedItemDataTable.DT_HoldedItemDataTable'"));
	}
	
	if(DataTable)
	{
		if(FWeaponBasicInfo* FoundWeaponInfo = DataTable->FindRow<FWeaponBasicInfo>(ID, ""))
		{
			Icon = FoundWeaponInfo->Icon;
			Name = FText::FromName(FoundWeaponInfo->Name);
			ItemType = FText::FromName(FoundWeaponInfo->ItemType);
			
			if(FoundWeaponInfo->ItemType == "None")
			{
				BulletNumBox->SetVisibility(ESlateVisibility::Hidden);
				ItemQuantityBox->SetVisibility(ESlateVisibility::Hidden);
			}
			else if(FoundWeaponInfo->ItemType == "Plant_Attack")
			{
				BulletNumBox->SetVisibility(ESlateVisibility::Visible);
				ItemQuantityBox->SetVisibility(ESlateVisibility::Hidden);
				
				MaxBulletNum = FoundWeaponInfo->Clips;
			}
			else if(FoundWeaponInfo->ItemType == "Plant_Defense")
			{
				BulletNumBox->SetVisibility(ESlateVisibility::Hidden);
				ItemQuantityBox->SetVisibility(ESlateVisibility::Hidden);
			}
			else if(FoundWeaponInfo->ItemType == "Item_Buff" || FoundWeaponInfo->ItemType == "Item_Damage")
			{
				BulletNumBox->SetVisibility(ESlateVisibility::Hidden);
				ItemQuantityBox->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UPVZ3DHoldedItemWidget::CurrentBulletNumChanged(FText NewBulletNum)
{
	CurrentBulletNum = NewBulletNum;
	UE_LOG(LogTemp, Warning, TEXT("CurrentBulletNum Changed: %s"), *CurrentBulletNum.ToString());
}

