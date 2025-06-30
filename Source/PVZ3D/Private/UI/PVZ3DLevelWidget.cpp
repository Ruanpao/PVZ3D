// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PVZ3DLevelWidget.h"
#include "Gamemode/PVZ3DGameState.h"

void UPVZ3DLevelWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
		
		if(APVZ3DGameState* GameState = Cast<APVZ3DGameState>(GetWorld()->GetGameState()))
		{
			GameState->OnCurrentWavesChanged.AddUObject(this, &UPVZ3DLevelWidget::UpdateCurrentWave);

			GameState->OnTotalWavesChanged.AddUObject(this, &UPVZ3DLevelWidget::UpdateTotalWaves);

			GameState->OnRemaningEnemiesChanged.AddUObject(this, &UPVZ3DLevelWidget::UpdateRemainingEnemy);
		}
	
}

void UPVZ3DLevelWidget::UpdateCurrentWave(int32 NewCurrentWave)
{
	CurrentWave = NewCurrentWave;
}

void UPVZ3DLevelWidget::UpdateRemainingEnemy(int32 NewRemainingEnemies)
{
	RemainingEnemy = NewRemainingEnemies;
}

void UPVZ3DLevelWidget::UpdateTotalWaves(int32 NewTotalWaves)
{
	TotalWaves = NewTotalWaves;
}