// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DRouteNode.h"

// Sets default values
APVZ3DRouteNode::APVZ3DRouteNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RouteID=-1;
	OrderIndex=-1;

}

// Called when the game starts or when spawned
void APVZ3DRouteNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DRouteNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

