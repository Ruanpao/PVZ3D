// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DRouteNode.h"
#include "PVZ3DRouteManager.generated.h"

UCLASS()
class PVZ3D_API APVZ3DRouteManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APVZ3DRouteManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	TArray<APVZ3DRouteNode*> AllRouteNodes;

	UFUNCTION(BlueprintCallable,Category="Route")
	TArray<APVZ3DRouteNode*> GetRouteNodesByID(int RouteID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
