// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DEnemy.h"
#include "PVZ3DEnemySpawnPoint.generated.h"

UCLASS()
class PVZ3D_API APVZ3DEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APVZ3DEnemySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	float CooldownTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	TSubclassOf<APVZ3DEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Route")
	int RouteID;

	UFUNCTION(BlueprintCallable, Category="AI", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true", AdvancedDisplay = "Owner"))
	ACharacter* PVZ3DSpawnEnemyFromClass(UObject* WorldContextObject, TSubclassOf<APVZ3DEnemy> PVZ3DEnemyClass, FVector Location, int EnemyRouteID, int BehaviortreeID, FRotator Rotation = FRotator::ZeroRotator, bool bNoCollisionFail = false);


	
	
};
