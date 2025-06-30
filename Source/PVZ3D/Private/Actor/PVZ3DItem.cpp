// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DItem.h"

APVZ3DItem::APVZ3DItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;
    
}

void APVZ3DItem::BeginPlay()
{
	Super::BeginPlay();

	ItemOwner =GetOwner();
}

void APVZ3DItem::StartUse()
{
}

void APVZ3DItem::StopUse()
{
}

AActor* APVZ3DItem::GetItemOwner() const
{
	return ItemOwner;
}

void APVZ3DItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}