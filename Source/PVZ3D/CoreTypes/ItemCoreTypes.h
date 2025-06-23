#pragma once
#include "Engine/DataTable.h"
#include "ItemCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemBasicInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	FName Name = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	UTexture2D* Icon = nullptr;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	bool Stackable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	int32 MaxStackNum = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	bool IsInstance = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	FText Description = FText::FromString("Invalid Description");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	FText Information = FText::FromString("Invalid Information");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItemInfo")
	FText ItemType = FText::FromString("Invalid ItemType");
};

USTRUCT(BlueprintType)
struct FItemInInventory
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SlotInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SlotInfo")
	int32 Quantity = 0;
};