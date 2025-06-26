// PVZ3DEnemySpawnPointManager.cpp
#include "AI/PVZ3DEnemySpawnPointManager.h"
#include "AI/PVZ3DEnemySpawnPoint.h"
#include "Engine/World.h"
#include "Engine/TimerHandle.h"
#include "Engine/DataTable.h"

APVZ3DEnemySpawnPointManager::APVZ3DEnemySpawnPointManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentWaveID = 0;             // int类型
    bCanNextWave = true;
    bIsSpawning = false;
    TotalEnemiesInWave = 0;        // int类型
    SpawnedEnemiesCount = 0;       // int类型
    CurrentRouteID = 0;            // int类型
}

void APVZ3DEnemySpawnPointManager::BeginPlay()
{
    Super::BeginPlay();
    LoadWaveDataFromDataTable();
    //StartWave(1); // 游戏开始时自动生成第一波
}

void APVZ3DEnemySpawnPointManager::BeginDestroy()
{
    Super::BeginDestroy();
    if (GetWorld())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_SpawnNextEnemy);
    }
}

void APVZ3DEnemySpawnPointManager::NotifyActorOnClicked(FKey ButtonPressed)
{
    Super::NotifyActorOnClicked(ButtonPressed);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Next Wave Button Clicked"));
    if(EnemySpawnPoint1->SpawnCompleted&&EnemySpawnPoint2->SpawnCompleted)
    {
        OnWaveSpawnComplete();
    }
    NextWave();
}

void APVZ3DEnemySpawnPointManager::LoadWaveDataFromDataTable()
{
    if (!WavesInfoDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("WaveDataTable is not set!"));
        return;
    }

    TArray<FName> RowNames = WavesInfoDataTable->GetRowNames();
    AllWaveRows.Empty();
    WavesInfoDataTable->GetAllRows(TEXT("WaveDataContext"), AllWaveRows);
    
    // 按RouteID分组存储波次数据
    RouteWaves.Empty();
    for (FWaveDataRow* Row : AllWaveRows)
    {
        if (Row)
        {
            RouteWaves.FindOrAdd(Row->RouteID).Add(Row); // 使用int类型RouteID作为键
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Loaded %d wave rows, grouped into %d routes"), 
           AllWaveRows.Num(), RouteWaves.Num());

    // 分配数据给SpawnPoint
    if (EnemySpawnPoint1)
    {
        if (RouteWaves.Contains(1))
        {
            for (const auto& Row : RouteWaves[1])
            {
                EnemySpawnPoint1->AssignedWaves.Add(*Row);
            }
        }
    }

    if (EnemySpawnPoint2)
    {
        if (RouteWaves.Contains(2))
        {
            for (const auto& Row : RouteWaves[2])
            {
                EnemySpawnPoint2->AssignedWaves.Add(*Row);
            }
        }
    }
}

void APVZ3DEnemySpawnPointManager::NextWave()
{
    if (!bCanNextWave)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot next wave: Current wave %d is still spawning"), CurrentWaveID);
        return;
    }
    
    int MaxWaveID = 0;
    for (FWaveDataRow* Row : AllWaveRows)
    {
        if (Row) MaxWaveID = FMath::Max(MaxWaveID, Row->WaveID);
    }
    
    int NextWaveID = CurrentWaveID + 1;
    if (NextWaveID <= MaxWaveID) 
        StartWave(NextWaveID);
    else 
        UE_LOG(LogTemp, Warning, TEXT("No more waves available"));
}

void APVZ3DEnemySpawnPointManager::StartWave(int WaveID)
{
    if (bIsSpawning) return;

    // 重置状态
    bIsSpawning = true;
    bCanNextWave = false;
    CurrentWaveID = WaveID;
    SpawnedEnemiesCount = 0;
    TotalEnemiesInWave = 0;
    
    // 计算总敌人数（按路线累加）
    for (auto& RoutePair : RouteWaves)
    {
        for (FWaveDataRow* Row : RoutePair.Value)
        {
            if (Row->WaveID == WaveID)
            {
                TArray<FString> Tokens;
                Row->EnemiesToSpawn.ParseIntoArray(Tokens, TEXT(" "), true);
                for (const FString& Token : Tokens)
                {
                    if (Token.IsEmpty() || Token == TEXT("-")) continue;
                    TArray<FString> Parts;
                    Token.ParseIntoArray(Parts, TEXT("*"), true);
                    TotalEnemiesInWave += (Parts.Num() > 1) ? FCString::Atoi(*Parts[1]) : 1;
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Starting Wave %d with %d enemies"), WaveID, TotalEnemiesInWave);

    // 让SpawnPoint开始生成敌人
    if (EnemySpawnPoint1)
    {
        EnemySpawnPoint1->StartWave(WaveID);
    }

    if (EnemySpawnPoint2)
    {
        EnemySpawnPoint2->StartWave(WaveID);
    }
}

void APVZ3DEnemySpawnPointManager::OnWaveSpawnComplete()
{
    bIsSpawning = false;
    bCanNextWave = true;
    UE_LOG(LogTemp, Warning, TEXT("Wave %d completed!"), CurrentWaveID);
}

int APVZ3DEnemySpawnPointManager::GetUniqueTimerKeySuffix()
{
    static int Suffix = 0;
    return ++Suffix; // 返回int类型
}