// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DItem.h"

// Sets default values
APVZ3DItem::APVZ3DItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APVZ3DItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

