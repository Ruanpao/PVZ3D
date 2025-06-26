// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/PVZ3DGamemode.h"
#include "AI/PVZ3DHome.h"
#include "Kismet/GameplayStatics.h"

APVZ3DGamemode::APVZ3DGamemode()
{
	UE_LOG(LogTemp,Error, TEXT("Kousaka Honoka"));

}

void APVZ3DGamemode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Gamemode: Game Started!"));
}

void APVZ3DGamemode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Gamemode: Game Ended!"));
}

void APVZ3DGamemode::GameWin()
{
	UE_LOG(LogTemp, Warning, TEXT("Gamemode: Game Won!"));
}

void APVZ3DGamemode::GameLose()
{
	UE_LOG(LogTemp, Warning, TEXT("Gamemode: Game Lost!"));
}
