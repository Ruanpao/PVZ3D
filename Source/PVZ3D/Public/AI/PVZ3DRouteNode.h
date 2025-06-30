// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DRouteNode.generated.h"

UCLASS()
class PVZ3D_API APVZ3DRouteNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APVZ3DRouteNode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	int RouteID;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	int OrderIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
