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

UCLASS()
class PVZ3D_API APVZ3DGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    // 当前关卡状态（可复制到客户端）
    UPROPERTY(ReplicatedUsing=OnRep_LevelState, BlueprintReadOnly, Category="Level")
    ELevelState CurrentLevelState = ELevelState::None;

    // 关卡计时器（从游戏开始的秒数）
    UPROPERTY(Replicated, BlueprintReadOnly, Category="Level")
    float LevelTimeElapsed = 0.f;

    // 关卡总时间限制（0表示无限制）
    UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category="Level")
    float LevelTimeLimit = 300.f; // 默认5分钟
    
    UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category="Level")
    float HomeCurrentHealth;
    
    // 关卡状态变化委托
    FOnLevelStateChanged OnLevelStateChanged;

    // 游戏结束委托
    FOnGameEnd OnGameEnd;

    // 服务器端：设置关卡状态
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetLevelState(ELevelState NewState);

    // 服务器端：开始关卡
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStartLevel();

    // 服务器端：暂停关卡
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerPauseLevel();

    // 服务器端：继续关卡
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerResumeLevel();

    // 服务器端：完成关卡
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerCompleteLevel();

    // 服务器端：失败关卡
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFailLevel();

    // 检查关卡是否已开始
    UFUNCTION(BlueprintCallable, Category="Level")
    bool IsLevelInProgress() const;

    // 检查关卡是否已完成
    UFUNCTION(BlueprintCallable, Category="Level")
    bool IsLevelCompleted() const;

    UFUNCTION(BlueprintCallable, Category="Level")
    bool IsLevelFailed() const;

    // 获取剩余时间（蓝图友好）
    UFUNCTION(BlueprintCallable, Category="Level")
    float GetRemainingTime() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameState")
    float HomeMaxHealth = 100.f;
    
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

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    // 当LevelState复制到客户端时调用
    UFUNCTION()
    void OnRep_LevelState(ELevelState OldState);

    // 更新关卡计时器
    void UpdateLevelTimer(float DeltaTime);

    int TotalWaves;
    int CurrentWave;
    int RemainingEnemies;
    int TotalEnemiesInCurrentWave;
};


