#pragma once
#include "HealCoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FAutoHeal
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
	bool IsAutoHeal = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "IsAutoHeal"))
	float AutoHeal_TimeDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "IsAutoHeal"))
	float AutoHeal_Amount = 10.0f;
};

USTRUCT(BlueprintType)
struct FSteadyHeal
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal")
	bool IsSteadyHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal", meta = (EditCondition = "IsSteadyHeal"))
	float SteadyHeal_Amount = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal", meta = (EditCondition = "IsSteadyHeal"))
	float SteadyHeal_Time = 0.0f;
};

USTRUCT(BlueprintType)
struct FInstantHeal
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal")
	bool IsInstantHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal", meta = (EditCondition = "IsInstantHeal"))
	float InstantHeal_TimeDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal", meta = (EditCondition = "IsInstantHeal"))
	float InstantHeal_Amount = 0.0f;
};