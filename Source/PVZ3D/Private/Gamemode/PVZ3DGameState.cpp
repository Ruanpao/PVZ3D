// PVZ3DGameState.cpp
#include "Gamemode/PVZ3DGameState.h"
#include "AI/PVZ3DHome.h"
#include "AI/PVZ3DEnemySpawnPointManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APVZ3DGameState::BeginPlay()
{
    Super::BeginPlay();
    
    if (HasAuthority())
    {
        ServerSetLevelState(ELevelState::Loading);
    }

    TArray<AActor*> HomeActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DHome::StaticClass(), HomeActors);
    
    if (HomeActors.Num() > 0)
    {
        APVZ3DHome* Home = Cast<APVZ3DHome>(HomeActors[0]);
        if (Home)
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
        }
    }
    
    CurrentWave = 0;
}

void APVZ3DGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 只有服务器更新计时器
    if (HasAuthority() && IsLevelInProgress())
    {
        UpdateLevelTimer(DeltaTime);
        
        // 检查时间是否用完
        if (LevelTimeLimit > 0.f && LevelTimeElapsed >= LevelTimeLimit)
        {
            ServerFailLevel();
        }
    }
}

void APVZ3DGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(APVZ3DGameState, CurrentLevelState);
    DOREPLIFETIME(APVZ3DGameState, LevelTimeElapsed);
    DOREPLIFETIME(APVZ3DGameState, LevelTimeLimit);
}

void APVZ3DGameState::ServerSetLevelState_Implementation(ELevelState NewState)
{
    if (CurrentLevelState != NewState)
    {
        ELevelState OldState = CurrentLevelState;
        CurrentLevelState = NewState;
        
        // 触发状态变化事件
        OnLevelStateChanged.Broadcast(NewState);
        
        UE_LOG(LogTemp, Warning, TEXT("Level state changed to: %s"), *UEnum::GetValueAsString(NewState));
    }
}

bool APVZ3DGameState::ServerSetLevelState_Validate(ELevelState NewState)
{
    return true; // 可以添加状态转换验证逻辑
}

void APVZ3DGameState::ServerStartLevel_Implementation()
{
    ServerSetLevelState(ELevelState::InProgress);
    LevelTimeElapsed = 0.f;
}

bool APVZ3DGameState::ServerStartLevel_Validate()
{
    return true;
}

void APVZ3DGameState::ServerPauseLevel_Implementation()
{
    if (IsLevelInProgress())
    {
        ServerSetLevelState(ELevelState::Paused);
    }
}

bool APVZ3DGameState::ServerPauseLevel_Validate()
{
    return true;
}

void APVZ3DGameState::ServerResumeLevel_Implementation()
{
    if (CurrentLevelState == ELevelState::Paused)
    {
        ServerSetLevelState(ELevelState::InProgress);
    }
}

bool APVZ3DGameState::ServerResumeLevel_Validate()
{
    return true;
}

void APVZ3DGameState::ServerCompleteLevel_Implementation()
{
    if (IsLevelInProgress())
    {
        ServerSetLevelState(ELevelState::Completed);
        OnGameEnd.Broadcast();
    }
}

bool APVZ3DGameState::ServerCompleteLevel_Validate()
{
    return true;
}

void APVZ3DGameState::ServerFailLevel_Implementation()
{
    if (IsLevelInProgress())
    {
        ServerSetLevelState(ELevelState::Failed);
        OnGameEnd.Broadcast();
    }
}

bool APVZ3DGameState::ServerFailLevel_Validate()
{
    return true;
}

bool APVZ3DGameState::IsLevelInProgress() const
{
    return CurrentLevelState == ELevelState::InProgress;
}

bool APVZ3DGameState::IsLevelCompleted() const
{
    return CurrentLevelState == ELevelState::Completed;
}

bool APVZ3DGameState::IsLevelFailed() const
{
    return CurrentLevelState == ELevelState::Failed;
}

float APVZ3DGameState::GetRemainingTime() const
{
    if (LevelTimeLimit <= 0.f) return 0.f;
    return FMath::Max(0.f, LevelTimeLimit - LevelTimeElapsed);
}

void APVZ3DGameState::UpdateHomeHealth(float NewHealth)
{
    // 更新Home的血量
    HomeCurrentHealth = NewHealth;
    UE_LOG(LogTemp,Error,TEXT("Gamestate: OnHomeHealthChanged, %f"), HomeCurrentHealth);
}

void APVZ3DGameState::UpdateTotalWave(int AllWaves)
{
    TotalWaves=AllWaves;
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Total Waves updated to: %d"), TotalWaves);
}

void APVZ3DGameState::UpdateCurrentWaveID(int NewWaveID)
{
    CurrentWave=NewWaveID;
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
    UE_LOG(LogTemp, Warning, TEXT("Hanazono Tae Remaining Enemies updated to: %d"), RemainingEnemies);
}

void APVZ3DGameState::OnHomeDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("GAMESTATE: Home has been destroyed!"));
}

void APVZ3DGameState::OnRep_LevelState(ELevelState OldState)
{
    // 客户端接收到状态变更时触发
    OnLevelStateChanged.Broadcast(CurrentLevelState);
}

void APVZ3DGameState::UpdateLevelTimer(float DeltaTime)
{
    LevelTimeElapsed += DeltaTime;
}