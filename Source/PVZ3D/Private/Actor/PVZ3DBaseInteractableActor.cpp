// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DBaseInteractableActor.h"

// Sets default values
APVZ3DBaseInteractableActor::APVZ3DBaseInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APVZ3DBaseInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DBaseInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

