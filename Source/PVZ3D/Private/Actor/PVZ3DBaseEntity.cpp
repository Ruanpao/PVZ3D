// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor\PVZ3DBaseEntity.h"

// Sets default values
APVZ3DBaseEntity::APVZ3DBaseEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APVZ3DBaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DBaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APVZ3DBaseEntity::MoveForward(const float Val)
{
}

void APVZ3DBaseEntity::MoveRight(const float Val)
{
}

void APVZ3DBaseEntity::LookUp(const float Val)
{
}

void APVZ3DBaseEntity::LookRight(const float Val)
{
}

void APVZ3DBaseEntity::Attack()
{
}

void APVZ3DBaseEntity::Interact()
{
}

void APVZ3DBaseEntity::StartRun()
{
}

void APVZ3DBaseEntity::StopRun()
{
}

void APVZ3DBaseEntity::UpdateCurrentWeaponID()
{
}

// Called to bind functionality to input
void APVZ3DBaseEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

