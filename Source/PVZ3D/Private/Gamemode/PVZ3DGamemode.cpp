// PVZ3DGamemode.cpp
#pragma once


#include "Gamemode/PVZ3DGamemode.h"
#include "Character/PVZ3DPlayer.h"
#include "Gamemode/PVZ3DGameState.h"
#include "Kismet/GameplayStatics.h"

APVZ3DGamemode::APVZ3DGamemode()
{
	// 设置默认的GameState类
	GameStateClass = APVZ3DGameState::StaticClass();
}

void APVZ3DGamemode::BeginPlay()
{
	Super::BeginPlay();
    
	if (GameState)
	{
		// 示例：当关卡完成时保存游戏进度
		UE_LOG(LogTemp, Warning, TEXT("PVZ3DGamode Beginplay gamestate"));
	}

	
	StartLevel();
}

void APVZ3DGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APVZ3DGamemode::StartLevel()
{
	if (GameState && !Cast<APVZ3DGameState>(GameState)->IsLevelInProgress())
	{
		Cast<APVZ3DGameState>(GameState)->ServerStartLevel();
	}
}

void APVZ3DGamemode::PauseLevel()
{
	if (GameState && Cast<APVZ3DGameState>(GameState)->IsLevelInProgress())
	{
		Cast<APVZ3DGameState>(GameState)->ServerPauseLevel();
	}
}

void APVZ3DGamemode::ResumeLevel()
{
	if (GameState && Cast<APVZ3DGameState>(GameState)->CurrentLevelState == ELevelState::Paused)
	{
		Cast<APVZ3DGameState>(GameState)->ServerResumeLevel();
	}
}

void APVZ3DGamemode::CompleteLevel()
{
	if (GameState && Cast<APVZ3DGameState>(GameState)->IsLevelInProgress())
	{
		Cast<APVZ3DGameState>(GameState)->ServerCompleteLevel();
	}
}
void APVZ3DGamemode::OnPlayerDied(APlayerController* PlayerController)
{
	if (!PlayerController || !PlayerController->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: 玩家控制器无效，尝试重新查找玩家"));
        
		// 尝试重新查找玩家控制器
		TArray<AActor*> PlayerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DPlayer::StaticClass(), PlayerActors);
		for (AActor* Actor : PlayerActors)
		{
			APVZ3DPlayer* Player = Cast<APVZ3DPlayer>(Actor);
			if (Player && Player->Controller)
			{
				PlayerController = Cast<APlayerController>(Player->Controller);
				break;
			}
		}
        
		if (!PlayerController || !PlayerController->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode: 无法找到有效玩家控制器，放弃复活"));
			return;
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("GameMode: 玩家死亡，5秒后复活"));
    
	// 使用 lambda 捕获控制器，并在复活时再次验证
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, WeakController = TWeakObjectPtr<APlayerController>(PlayerController)]()
		{
			APlayerController* ValidController = WeakController.Get();
			if (ValidController && ValidController->IsValidLowLevel())
			{
				RestartPlayer(ValidController);
				UE_LOG(LogTemp, Warning, TEXT("GameMode: 玩家复活成功"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("GameMode: 玩家控制器已无效，无法复活"));
			}
		},
		5.0f,
		false
	);
}

void APVZ3DGamemode::DelayedRestartPlayer(APlayerController* PlayerController)
{
	if (!PlayerControllerToRespawn.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: 玩家控制器已无效，无法复活"));
		return;
	}
    
	// 调用基类方法重启玩家
	RestartPlayer(PlayerControllerToRespawn.Get());
	UE_LOG(LogTemp, Warning, TEXT("GameMode: 玩家复活成功"));
}