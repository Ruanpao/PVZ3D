// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DEnemySpawnPointManager.generated.h"

class APVZ3DEnemySpawnPoint;


UCLASS()
class PVZ3D_API APVZ3DEnemySpawnPointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APVZ3DEnemySpawnPointManager();

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	APVZ3DEnemySpawnPoint* EnemySpawnPoint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	APVZ3DEnemySpawnPoint* EnemySpawnPoint2;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
