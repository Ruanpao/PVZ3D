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
	FName ItemType = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ItenInfo")
	int32 Price = 0;
};

USTRUCT(BlueprintType)
struct FItemInInventory
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SlotInfo")
	int32 Index = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SlotInfo")
	FName ID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SlotInfo")
	int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FShopStack : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName LevelName = "00";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack01 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack02 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack03 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack04 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack05 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack06 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack07 = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ShopStack")
	FName Stack08 = "0000";
};