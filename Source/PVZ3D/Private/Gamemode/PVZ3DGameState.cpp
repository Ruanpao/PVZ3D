// PVZ3DGameState.cpp
#include "Gamemode/PVZ3DGameState.h"
#include "AI/PVZ3DHome.h"
#include "AI/PVZ3DEnemySpawnPointManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APVZ3DGameState::BeginPlay()
{
    Super::BeginPlay();
    
    TArray<AActor*> HomeActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DHome::StaticClass(), HomeActors);
    
    if (HomeActors.Num() > 0)
    {
        if (APVZ3DHome* Home = Cast<APVZ3DHome>(HomeActors[0]))
        {
            Home->OnHomeHealthChanged.AddDynamic(this, &APVZ3DGameState::UpdateHomeHealth);
            
            Home->HomeDeathDelegate.AddDynamic(this, &APVZ3DGameState::OnHomeDeath);
            
            UpdateHomeHealth(Home->GetHomeCurrentHealth());
            UE_LOG(LogTemp, Warning, TEXT("APVZ3DGameState::BeginPlay - Home found and events bound!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Ye Taiga"));
    }

    TArray<AActor*> EnemySpawnPointManager;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DEnemySpawnPointManager::StaticClass(), EnemySpawnPointManager);
    if (EnemySpawnPointManager.Num()>0)
    {
        APVZ3DEnemySpawnPointManager* SpawnPointManager = Cast<APVZ3DEnemySpawnPointManager>(EnemySpawnPointManager[0]);
        if(SpawnPointManager)
        {
            SpawnPointManager->TotalEnemiesInWaveChanged.AddDynamic(this, &APVZ3DGameState::UpdateTotalEnemiesInCurrentWave);
            SpawnPointManager->TotalWavesChanged.AddDynamic(this, &APVZ3DGameState::UpdateTotalWave);
            SpawnPointManager->CurrentWaveChanged.AddDynamic(this, &APVZ3DGameState::UpdateCurrentWaveID);
            SpawnPointManager->CurrentWaveTimeLimitChanged.AddDynamic(this, &APVZ3DGameState::UpdateTotalTime);
        }
    }
    
    CurrentWave = 0;
}

void APVZ3DGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APVZ3DGameState::UpdateHomeHealth(float NewHealth)
{
    // 更新Home的血量
    HomeCurrentHealth = NewHealth;

    OnBaseHealthChanged.Broadcast(HomeCurrentHealth);
    
    UE_LOG(LogTemp,Error,TEXT("Gamestate: OnHomeHealthChanged, %f"), HomeCurrentHealth);
}

void APVZ3DGameState::UpdateTotalWave(int AllWaves)
{
    TotalWaves=AllWaves;
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Total Waves updated to: %d"), TotalWaves);

    OnTotalWavesChanged.Broadcast(TotalWaves);
}

void APVZ3DGameState::UpdateCurrentWaveID(int NewWaveID)
{
    CurrentWave=NewWaveID;

    OnCurrentWavesChanged.Broadcast(CurrentWave);
    
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Current Wave ID updated to: %d"), CurrentWave);
}

void APVZ3DGameState::UpdateTotalEnemiesInCurrentWave(int NewTotalEnemiesInCurrentWave)
{
    TotalEnemiesInCurrentWave = NewTotalEnemiesInCurrentWave;
    UpdateRemainingEnemies(RemainingEnemies + NewTotalEnemiesInCurrentWave); 
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Total Enemies in Current Wave updated to: %d"), TotalEnemiesInCurrentWave);
}

void APVZ3DGameState::EnemyDead()
{
    RemainingEnemies--;
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Enemy Dead! Remaining Enemies in Current Wave: %d"), RemainingEnemies);
}

void APVZ3DGameState::UpdateRemainingEnemies(int NewRemainingEnemies)
{
    RemainingEnemies = NewRemainingEnemies;

    OnRemaningEnemiesChanged.Broadcast(RemainingEnemies);
    
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Remaining Enemies updated to: %d"), RemainingEnemies);
}

void APVZ3DGameState::OnHomeDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("GAMESTATE: Home has been destroyed!"));
}

void APVZ3DGameState::UpdateTotalTime(int32 NewTotalTime)
{
    TotalTime = NewTotalTime;
}


void APVZ3DGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // 添加需要复制的属性（如果有）
    // DOREPLIFETIME(APVZ3DGameState, MyReplicatedProperty);
}