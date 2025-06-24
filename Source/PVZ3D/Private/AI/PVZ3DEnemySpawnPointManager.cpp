// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemySpawnPointManager.h"
#include "AI/PVZ3DEnemySpawnPoint.h"

// Sets default values
APVZ3DEnemySpawnPointManager::APVZ3DEnemySpawnPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APVZ3DEnemySpawnPointManager::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Point Clicked"));

	
}

// Called when the game starts or when spawned
void APVZ3DEnemySpawnPointManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DEnemySpawnPointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

