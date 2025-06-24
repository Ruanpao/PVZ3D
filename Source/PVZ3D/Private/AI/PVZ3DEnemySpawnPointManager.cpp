// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemySpawnPointManager.h"

// Sets default values
APVZ3DEnemySpawnPointManager::APVZ3DEnemySpawnPointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

