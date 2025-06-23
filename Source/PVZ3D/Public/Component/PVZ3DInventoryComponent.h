// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PVZ3D/CoreTypes/ItemCoreTypes.h"
#include "PVZ3DInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate)

USTRUCT()
struct FFindSlot
{
	GENERATED_BODY()

	bool FindSlot = false;

	int32 SlotIndex = -1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PVZ3D_API UPVZ3DInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPVZ3DInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	UDataTable* Datatable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InventoryComponent")
	TArray<FItemInInventory> Slot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InventoryComponent")
	int32 SlotSize = 10;

	FOnInventoryUpdate OnInventoryUpdate;
	
	void AddToInventory(const FName Item_ID, int32 Quantity);

	FFindSlot FindSlot(FName Item_ID);

	void AddOne(int32 Index, int32 Quantity);

	int32 AnyEmptySlotAvailable() const;

	void CreateNewSlot(FName Item_ID, int32 Index);

	void UpdateSlot();
};
