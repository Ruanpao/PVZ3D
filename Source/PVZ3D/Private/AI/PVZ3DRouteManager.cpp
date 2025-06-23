// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DRouteManager.h"

// Sets default values
APVZ3DRouteManager::APVZ3DRouteManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<APVZ3DRouteNode*> APVZ3DRouteManager::GetRouteNodesByID(int RouteID)
{
	TArray<APVZ3DRouteNode*> RouteNodes;
	for(APVZ3DRouteNode* Node : AllRouteNodes)
	{
		if(Node && Node->RouteID == RouteID)
		{
			RouteNodes.Add(Node);
		}
	}

	RouteNodes.Sort([](const APVZ3DRouteNode& A, const APVZ3DRouteNode& B)
	{
		return A.OrderIndex < B.OrderIndex;
	});

	return RouteNodes;
}

// Called when the game starts or when spawned
void APVZ3DRouteManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APVZ3DRouteManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

