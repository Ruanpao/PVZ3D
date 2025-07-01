// PVZ3DGameState.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PVZ3DGameState.generated.h"

// 关卡状态枚举
UENUM(BlueprintType)
enum class ELevelState : uint8
{
    None,               // 无状态
    Loading,            // 加载中
    WaitingToStart,     // 等待开始
    InProgress,         // 进行中
    Paused,             // 已暂停
    Completed,          // 已完成
    Failed              // 已失败
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelStateChanged, ELevelState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBaseHealthChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemaningEnemiesChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentWavesChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTotalWavesChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTotalTimeChanged , float);

UCLASS()
class PVZ3D_API APVZ3DGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category="Level")
    float HomeCurrentHealth;

    FOnBaseHealthChanged OnBaseHealthChanged;

    FOnCurrentWavesChanged OnCurrentWavesChanged;

    FOnRemaningEnemiesChanged OnRemaningEnemiesChanged;

    FOnTotalWavesChanged OnTotalWavesChanged;

    FOnTotalTimeChanged OnTotalTimeChanged;
    
    // 关卡状态变化委托
    FOnLevelStateChanged OnLevelStateChanged;

    // 游戏结束委托
    FOnGameEnd OnGameEnd;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState")
    float HomeMaxHealth = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState")
    float TotalTime = 0.0f;

    UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "GameState")
    float RemainTime = 0.0f;

    UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "GameState")
    int32 CurrentWave = 0;

    UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "GameState")
    int32 TotalWaves = 0;

    UPROPERTY(EditDefaultsOnly , BlueprintReadOnly, Category = "GameState")
    int32 RemainingEnemies = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState")
    int32 TotalEnemiesInCurrentWave = 0;
    
    UFUNCTION(BlueprintCallable, Category="Level")
    void UpdateHomeHealth(float NewHealth);

    UFUNCTION(BlueprintCallable, Category="Level")
    void UpdateTotalWave(int NewAllWaves);

    UFUNCTION(BlueprintCallable, Category="Level")
    void UpdateCurrentWaveID(int NewWaveID);
    
    UFUNCTION(BlueprintCallable, Category="Level")
    void UpdateTotalEnemiesInCurrentWave(int NewTotalEnemiesInCurrentWave);

    UFUNCTION(BlueprintCallable, Category="Level")
    void EnemyDead();
    
    UFUNCTION(BlueprintCallable, Category="Level")
    void UpdateRemainingEnemies(int NewRemainingEnemies);
    
    UFUNCTION()
    void OnHomeDeath();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void UpdateTotalTime(float NewTotalTime);
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};


