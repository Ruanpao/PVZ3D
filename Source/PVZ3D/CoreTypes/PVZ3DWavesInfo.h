#pragma once
#include "Engine/DataTable.h"
#include "PVZ3DWavesInfo.generated.h"

USTRUCT(BlueprintType)
struct FWavesInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	FName LevelID = "000";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	int WaveID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	FName RouteID = "-1";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	FString EnemiesToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	float SpawnInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WavesInfo")
	float TimeLimit = 100.0f;
};