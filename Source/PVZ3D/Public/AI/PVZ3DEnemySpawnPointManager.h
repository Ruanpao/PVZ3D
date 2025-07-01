// PVZ3DEnemySpawnPointManager.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3D/CoreTypes/PVZ3DWavesInfo.h"
#include "PVZ3DEnemySpawnPointManager.generated.h"

class APVZ3DEnemySpawnPoint;

DECLARE_MULTICAST_DELEGATE(FOnVictory1);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTotalEnemiesInWaveChanged, int,BoardcastTotalEnemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTotalWavesChanged, int, BoardcastTotalWaves);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWaveChanged, int, BoardcastCurrentWaveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentWaveTimeLimitChanged, float, CurrentWaveTimeLimit);

USTRUCT(BlueprintType)
struct FWaveDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	FName LevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	int WaveID;         // 使用int类型
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	int RouteID;        // 使用int类型

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	FString EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveData")
	float TimeLimit; // 使用float类型
	
};

UCLASS()
class PVZ3D_API APVZ3DEnemySpawnPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APVZ3DEnemySpawnPointManager();
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	FOnVictory1 OnVictory1;
	FTotalEnemiesInWaveChanged TotalEnemiesInWaveChanged; // 波次总敌人数变化委托
	FTotalWavesChanged TotalWavesChanged; // 波次总数变化委托
	FCurrentWaveChanged CurrentWaveChanged; // 当前波次变化委托
	FCurrentWaveTimeLimitChanged CurrentWaveTimeLimitChanged;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	/** 核心状态管理 */
	bool bIsSpawning;              // 是否正在生成敌人
	int TotalEnemiesInWave;        // 当前波次总敌人数（int类型）
	int SpawnedEnemiesCount;       // 已生成敌人数（int类型）
	int CurrentWaveID;             // 当前波次ID（int类型）
	int TotalWaves;
	
	/** 路线数据管理 */
	TArray<FWaveDataRow*> AllWaveRows;          // 从数据表加载的所有行
	TMap<int, TArray<FWaveDataRow*>> RouteWaves; // 按RouteID分组的波次数据（int类型键）
	
	/** 定时器管理 */
	FTimerHandle TimerHandle_WaveTimeout; // 波次超时定时器
	int GetUniqueTimerKeySuffix();               // 返回int类型

	/** 敌人生成逻辑 */
	void OnWaveSpawnComplete();
	void OnWaveTimeLimitReached();       // 波次超时回调
	float GetCurrentWaveTimeLimit(int WaveID); // 获取当前波的TimeLimit
	
public:	
	/** 公开接口 */
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void LoadWaveDataFromDataTable();
	
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void NextWave();
	
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave(int WaveID); // int类型参数

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void ForceNextWave();


	/** 可编辑属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	APVZ3DEnemySpawnPoint* EnemySpawnPoint1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	APVZ3DEnemySpawnPoint* EnemySpawnPoint2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* WavesInfoDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveStatus")
	int CurrentRouteID;      // 当前处理的路线ID（int类型）
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveStatus")
	bool bCanNextWave;
};