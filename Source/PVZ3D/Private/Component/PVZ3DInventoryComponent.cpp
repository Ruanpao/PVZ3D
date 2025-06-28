
#include "Component/PVZ3DInventoryComponent.h"
#include "UI/PVZ3DPlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventory, All, All);

UPVZ3DInventoryComponent::UPVZ3DInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Datatable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_ItemDataTable.DT_ItemDataTable'"));
}


void UPVZ3DInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateSlot();

	HoldedItem = Slot[0];

	if(APVZ3DPlayerHUD* HUD = Cast<APVZ3DPlayerHUD>(UGameplayStatics::GetPlayerController(this,0)->GetHUD()))
	{
		HUD->Buy.AddUObject(this, &UPVZ3DInventoryComponent::Buy);

		HUD->OnHoledSlotChanged.AddUObject(this, &UPVZ3DInventoryComponent::UpdateHoldedSlot);

		HUD->RemoveItem.AddUObject(this, &UPVZ3DInventoryComponent::RemoveFromInventory);
	}
}

bool UPVZ3DInventoryComponent::AddToInventory(const FName Item_ID, int32 Quantity)
{
	int32 LocalQuantityRemaining = Quantity;

	bool LocalHasFailed = false;

	while(LocalQuantityRemaining > 0 && !LocalHasFailed)
	{
		FFindSlot FindSlotResult = FindSlot(Item_ID);
		
		if(FindSlotResult.FindSlot)
		{
			AddOne(FindSlotResult.SlotIndex, 1);

			OnInventoryUpdate.Broadcast();

			if(FindSlotResult.SlotIndex == HoldedItem.Index)
			{
				UpdateHoldedSlot(FindSlotResult.SlotIndex);
			}

			LocalQuantityRemaining -= 1;
		}
		else if (AnyEmptySlotAvailable() >= 0)
		{
			int32 EmptySlotIndex = AnyEmptySlotAvailable();

			CreateNewSlot(Item_ID, EmptySlotIndex);

			OnInventoryUpdate.Broadcast();

			if(EmptySlotIndex == HoldedItem.Index)
			{
				UpdateHoldedSlot(EmptySlotIndex);
			}

			LocalQuantityRemaining -= 1;
		}
		else
		{
			LocalHasFailed = true;
		}
	}

	return LocalHasFailed;
}

FFindSlot UPVZ3DInventoryComponent::FindSlot(FName Item_ID)
{
	FFindSlot ReturnResult;
	ReturnResult.FindSlot = false;
	ReturnResult.SlotIndex = -1;
	
	for (int32 index = 0 ; index < SlotSize ; index++)
	{
		if(Item_ID == Slot[index].ID)
		{
			if(FItemBasicInfo* FoundItemInfo = Datatable->FindRow<FItemBasicInfo>(Item_ID , ""))
			{
				if(Slot[index].Quantity < FoundItemInfo->MaxStackNum)
				{
					ReturnResult.FindSlot = true;
					ReturnResult.SlotIndex = index;
					return ReturnResult;
				}
			}
			else
			{
				return ReturnResult;
			}
		}
	}
	return ReturnResult;
}

void UPVZ3DInventoryComponent::AddOne(int32 Index, int32 Quantity)
{
	Slot[Index].Quantity += Quantity;
}

int32 UPVZ3DInventoryComponent::AnyEmptySlotAvailable() const
{
	for (int32 index = 0 ; index < SlotSize ; index++)
	{
		if (Slot[index].Quantity == 0)
		{
			return index;
		}
	}

	return -1;
}

void UPVZ3DInventoryComponent::CreateNewSlot(FName Item_ID, int32 Index)
{
	Slot[Index].ID = Item_ID;
	Slot[Index].Quantity = 1;
}

void UPVZ3DInventoryComponent::UpdateSlot()
{
	int32 SlotIndex = 0;
	
	while(Slot.Num() < SlotSize)
	{
		FItemInInventory NewSlot;
		NewSlot.ID = "0000";
		NewSlot.Quantity = 0;
		NewSlot.Index = SlotIndex;
		
		Slot.Add(NewSlot);

		SlotIndex += 1;
	}
	OnInventoryUpdate.Broadcast();
}

void UPVZ3DInventoryComponent::RemoveFromInventory(int32 Index, bool RemoveAll, bool IsConsumed)
{
	if(Slot[Index].Quantity == 1 or RemoveAll)
	{
		if(IsConsumed)
		{
			DestroyAOldSlot(Index);
			UE_LOG(LogInventory, Error ,TEXT("I am Consumed"));

			OnInventoryUpdate.Broadcast();

			if(Index == HoldedItem.Index)
			{
				UpdateHoldedSlot(Index);
			}
		}
		else
		{
			DestroyAOldSlot(Index);

			OnInventoryUpdate.Broadcast();

			if(Index == HoldedItem.Index)
			{
				UpdateHoldedSlot(Index);

				UE_LOG(LogInventory , Warning , TEXT(" %d %d --- %s %d"),Index, HoldedItem.Index , *HoldedItem.ID.ToString(), HoldedItem.Quantity);
			}
		}
	}
	else
	{
		if(IsConsumed)
		{
			RemoveOne(Index , 1);
			UE_LOG(LogInventory, Error ,TEXT("I am Consumed"));

			OnInventoryUpdate.Broadcast();

			if(Index == HoldedItem.Index)
			{
				UpdateHoldedSlot(Index);
			}
		}
		else
		{
			RemoveOne(Index, 1);

			OnInventoryUpdate.Broadcast();
			
			if(Index == HoldedItem.Index)
			{
				UpdateHoldedSlot(Index);

				UE_LOG(LogInventory , Warning , TEXT(" %d %d --- %s %d"),Index, HoldedItem.Index , *HoldedItem.ID.ToString(), HoldedItem.Quantity);
			}
		}
	}
}

void UPVZ3DInventoryComponent::RemoveOne(int32 Index , int32 Quantity)
{
	Slot[Index].Quantity -= Quantity;
}

void UPVZ3DInventoryComponent::DestroyAOldSlot(int32 Index)
{
	Slot[Index].ID = "0000";
	Slot[Index].Quantity = 0;
}


void UPVZ3DInventoryComponent::Buy(FName ID , int32 Quantity , int32 Price)
{
	if(ID != "0000")
	{
		if(Gold >= Price)
		{
			if(AddToInventory(ID , Quantity) == false)
			{
				Gold -= Price;

				OnGoldChanged.Broadcast(Gold);
			}
			else
			{
				UE_LOG(LogInventory, Error, TEXT("INVENTORY IS FULL, CANNOT ADD ITEM: %s"), *ID.ToString());
			}
		}
		else
		{
			UE_LOG(LogInventory , Error, TEXT("NOT ENOUGH GOLD TO BUY ITEM: %s"), *ID.ToString());
		}
	}
}

int32 UPVZ3DInventoryComponent::GetCurrentGold()
{
	return Gold;
}

void UPVZ3DInventoryComponent::UpdateHoldedSlot(int Index)
{
	HoldedItem = Slot[Index];
	
	HoldedChanged.Broadcast(HoldedItem);

	UE_LOG(LogInventory , Warning , TEXT("HoldedItem Changed, ID : %s , Quantity : %d"), *HoldedItem.ID.ToString(), HoldedItem.Quantity);
}


