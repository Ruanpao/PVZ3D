// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemySpawnPoint.h"
#include "AI/PVZ3DEnemy.h"
#include "AIController.h"
#include "AI/PVZ3DEnemyController.h"
#include "Engine/World.h"
#include "Blueprint\AIBlueprintHelperLibrary.h"

// Sets default values
APVZ3DEnemySpawnPoint::APVZ3DEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APVZ3DEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	//PVZ3DSpawnEnemyFromClass(this, EnemyClass, GetActorLocation(), RouteID, EnemyID, BehaviorTreeID ,GetActorRotation(), true);
}

// Called every frame
void APVZ3DEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DEnemySpawnPoint::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Point Clicked"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *EnemyID.ToString());
	PVZ3DSpawnEnemyFromClass(this, EnemyClass, GetActorLocation(), RouteID, BehaviorTreeID, EnemyID  ,GetActorRotation(), true);
	
}

ACharacter* APVZ3DEnemySpawnPoint::PVZ3DSpawnEnemyFromClass(UObject* WorldContextObject,
                                                            TSubclassOf<APVZ3DEnemy> PVZ3DEnemyClass, FVector Location, int EnemyRouteID, FName EnemyBehaviortreeID,
                                                            FName SpawnEnemyID, FRotator Rotation, bool bNoCollisionFail)
{
	ACharacter* NewCharacter = NULL;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World && *PVZ3DEnemyClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = bNoCollisionFail ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		NewCharacter = World->SpawnActor<ACharacter>(PVZ3DEnemyClass, Location, Rotation, ActorSpawnParams);

		if (NewCharacter != NULL)
		{
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: 000Spawned Enemy with ID: %s"), *SpawnEnemyID.ToString());
			Cast<APVZ3DEnemy>(NewCharacter)->RouteID= EnemyRouteID;
			Cast<APVZ3DEnemy>(NewCharacter)->CurrentRouteNodes=Cast<APVZ3DEnemy>(NewCharacter)->RouteManager->GetRouteNodesByID(RouteID);
			Cast<APVZ3DEnemy>(NewCharacter)->EnemyID = SpawnEnemyID;
			UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with ID: %s"), *Cast<APVZ3DEnemy>(NewCharacter)->EnemyID.ToString());
			UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with SpawnEnemyID: %s"), *SpawnEnemyID.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: 111Spawned Enemy with ID: %s"), *Cast<APVZ3DEnemy>(NewCharacter)->EnemyID.ToString());
			Cast<APVZ3DEnemy>(NewCharacter)->UpdateEnemyImformation();

			
			UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with RouteID: %d"), EnemyRouteID);

			if (NewCharacter->Controller == NULL)
			{	// NOTE: SpawnDefaultController ALSO calls Possess() to possess the pawn (if a controller is successfully spawned).
				NewCharacter->SpawnDefaultController();
			}
			
			AAIController* AIController = Cast<AAIController>(NewCharacter->Controller);

			if (AIController != NULL)
			{
				Cast<APVZ3DEnemyController>(AIController)->RunPVZ3DEnemyBehaviorTree(EnemyBehaviortreeID);
			}
		}
	}

	return NewCharacter;
}

