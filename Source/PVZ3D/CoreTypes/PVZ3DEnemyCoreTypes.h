#pragma once
#include "Engine/DataTable.h"
#include "GenericTeamAgentInterface.h"
#include "PVZ3DEnemyCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FEnemyBasicInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName EnemyID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName EnemyName = "None";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName WeaponID = "0000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	float MaxHealth = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	FName BehaviourTreeID = "1";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	int Vecolity = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	double AttackRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly,Blueprintable, Category="TowerInfo")
	int AggroValue = 0;

	UPROPERTY(EditDefaultsOnly,Blueprintable, Category="TowerInfo")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TowerInfo")
	float AttackInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly,Blueprintable, Category="TowerInfo")
	FGenericTeamId TeamID = 0;
};