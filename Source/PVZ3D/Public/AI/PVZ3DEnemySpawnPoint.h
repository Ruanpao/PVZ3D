// PVZ3DEnemySpawnPoint.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DEnemy.h"
#include "AI/PVZ3DEnemySpawnPointManager.h"
#include "PVZ3DEnemySpawnPoint.generated.h"

UCLASS()
class PVZ3D_API APVZ3DEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APVZ3DEnemySpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	TSubclassOf<APVZ3DEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	int RouteID;       // 使用int类型

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	FName EnemyID;

	UFUNCTION(BlueprintCallable, Category="AI")
	ACharacter* PVZ3DSpawnEnemyFromClass(UObject* WorldContextObject, TSubclassOf<APVZ3DEnemy> PVZ3DEnemyClass, FVector Location, int EnemyRouteID, FName SpawnEnemyID, FRotator Rotation = FRotator::ZeroRotator, bool bNoCollisionFail = false);

	UPROPERTY()
	TArray<FWaveDataRow> AssignedWaves; // 分配给本SpawnPoint的波次数据，使用结构体引用

	int CurrentWaveIndex;

	void StartWave(int WaveID);
	void SpawnNextEnemy();
	void OnWaveSpawnComplete();
	FTimerHandle EnemySpawnTimer;

	bool SpawnCompleted;

private:
	int SpawnedEnemiesCount;
	TArray<FName> EnemiesToSpawn;
};