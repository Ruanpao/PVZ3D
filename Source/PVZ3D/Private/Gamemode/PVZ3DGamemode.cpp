// PVZ3DGamemode.cpp
#include "Gamemode/PVZ3DGamemode.h"
#include "Gamemode/PVZ3DGameState.h"

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