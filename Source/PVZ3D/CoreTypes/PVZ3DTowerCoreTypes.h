#pragma once
#include "Engine/DataTable.h"
#include "PVZ3DTowerCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FTowerBasicInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName TowerID = "000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName TowerName = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName WeaponID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	double Health = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName BehaviourTreeID = "1";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	int Height = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	double AttackRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly,Blueprintable, Category="TowerInfo")
	int AggroValue = 0;

	UPROPERTY(EditDefaultsOnly,Blueprintable, Category="TowerInfo")
	FGenericTeamId TeamID = 0;
};

USTRUCT(BlueprintType)
struct FTowerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerState")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerState")
	int32 CurrentAmmo;
};