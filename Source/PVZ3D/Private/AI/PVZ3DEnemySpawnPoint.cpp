// PVZ3DEnemySpawnPoint.cpp
#include "AI/PVZ3DEnemySpawnPoint.h"
#include "AI/PVZ3DEnemy.h"
#include "AIController.h"
#include "AI/PVZ3DEnemyController.h"
#include "AI/PVZ3DEnemySpawnPointManager.h"
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
    PVZ3DSpawnEnemyFromClass(this, EnemyClass, GetActorLocation(), RouteID, EnemyID, GetActorRotation(), true);
}

ACharacter* APVZ3DEnemySpawnPoint::PVZ3DSpawnEnemyFromClass(UObject* WorldContextObject,
                                                            TSubclassOf<APVZ3DEnemy> PVZ3DEnemyClass, FVector Location, int EnemyRouteID,
                                                            FName SpawnEnemyID, FRotator Rotation, bool bNoCollisionFail)
{
    UE_LOG(LogTemp,Warning,TEXT("RouteID:%d SUZUMIYAHARUHI"), EnemyRouteID);
    UE_LOG(LogTemp,Warning, TEXT("APVZ3DEnemySpawnPoint: PVZ3DSpawnEnemyFromClass called with EnemyID: %s"), *SpawnEnemyID.ToString());
    ACharacter* NewCharacter = NULL;
    if (!PVZ3DEnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("APVZ3DEnemySpawnPoint: PVZ3DEnemyClass is NULL!"));
        return NewCharacter;
    }
    // 检查World是否有效
    if (WorldContextObject==nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("WorldContextObject is null!"));
        return nullptr;
    }

    // 检查类是否有效
    if (!PVZ3DEnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Enemy class is null!"));
        return nullptr;
    }
    
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (World && *PVZ3DEnemyClass)
    {
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = bNoCollisionFail ? ESpawnActorCollisionHandlingMethod::AlwaysSpawn : ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        NewCharacter = World->SpawnActor<ACharacter>(PVZ3DEnemyClass, Location, Rotation, ActorSpawnParams);

        if (NewCharacter != NULL)
        {
            Cast<APVZ3DEnemy>(NewCharacter)->RouteID = EnemyRouteID;
            Cast<APVZ3DEnemy>(NewCharacter)->CurrentRouteNodes = Cast<APVZ3DEnemy>(NewCharacter)->RouteManager->GetRouteNodesByID(RouteID);
            Cast<APVZ3DEnemy>(NewCharacter)->EnemyID = SpawnEnemyID;
            UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with ID: %s"), *Cast<APVZ3DEnemy>(NewCharacter)->EnemyID.ToString());
            UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with SpawnEnemyID: %s"), *SpawnEnemyID.ToString());
            Cast<APVZ3DEnemy>(NewCharacter)->UpdateEnemyImformation();

            UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemySpawnPoint: Spawned Enemy with RouteID: %d"), EnemyRouteID);

            if (NewCharacter->Controller == NULL)
            {   // NOTE: SpawnDefaultController ALSO calls Possess() to possess the pawn (if a controller is successfully spawned).
                UE_LOG(LogTemp,Warning,TEXT("RouteID:%d NagatoYuki SpawnDefaultController"), EnemyRouteID);
                NewCharacter->SpawnDefaultController();
            }
        }
    }
UE_LOG(LogTemp,Warning,TEXT("NagatoYuki RouteID:%d"), EnemyRouteID);
    return NewCharacter;
}

void APVZ3DEnemySpawnPoint::StartWave(int WaveID)
{
    SpawnCompleted=0;
    // 找到当前波次的数据
    for (const auto& WaveData : AssignedWaves)
    {
        if (WaveData.WaveID == WaveID)
        {
            // 解析敌人数据
            TArray<FString> Tokens;
            WaveData.EnemiesToSpawn.ParseIntoArray(Tokens, TEXT(" "), true);
            EnemiesToSpawn.Empty();
            for (const FString& Token : Tokens)
            {
                if (Token.IsEmpty() || Token == TEXT("-")) continue;
                TArray<FString> Parts;
                Token.ParseIntoArray(Parts, TEXT("*"), true);
                FName NewEnemyID = FName(*Parts[0]);
                int Count = (Parts.Num() > 1) ? FCString::Atoi(*Parts[1]) : 1;
                for (int i = 0; i < Count; ++i)
                {
                    EnemiesToSpawn.Add(NewEnemyID);
                }
            }

            SpawnedEnemiesCount = 0;
            UE_LOG(LogTemp, Warning, TEXT("Haruka RouteID:%d Starting wave %d with %d enemies to spawn"),RouteID ,WaveID, EnemiesToSpawn.Num());
            SpawnNextEnemy();
            UE_LOG(LogTemp, Warning, TEXT("Miki RouteID:%d Starting wave %d with %d enemies to spawn"),RouteID, WaveID, EnemiesToSpawn.Num());
            break;
        }
    }
}

void APVZ3DEnemySpawnPoint::SpawnNextEnemy()
{
    UE_LOG(LogTemp, Warning, TEXT("RouteID:%d YamadaRyou0"), RouteID);
    if (EnemiesToSpawn.Num() > 0)
    {
        // 修改局部变量名，避免和类成员重名
        FName NextEnemyID = EnemiesToSpawn[0];
        EnemiesToSpawn.RemoveAt(0);
        UE_LOG(LogTemp, Warning, TEXT("RouteID:%d YamadaRyou1"), RouteID);
        PVZ3DSpawnEnemyFromClass(this, EnemyClass, GetActorLocation(), RouteID, NextEnemyID, GetActorRotation(), true);
        
        UE_LOG(LogTemp, Warning, TEXT("RouteID:%d YamadaRyou2"), RouteID);
        FTimerDelegate SpawnDelegate = FTimerDelegate::CreateUObject(
                        this, 
                        &APVZ3DEnemySpawnPoint::SpawnNextEnemy
                    );
        UE_LOG(LogTemp, Warning, TEXT("RouteID:%d YamadaRyou3"), RouteID);

        if (SpawnDelegate.IsBound())
        {
             GetWorldTimerManager().SetTimer(
                  EnemySpawnTimer,
                     SpawnDelegate,
                     AssignedWaves[CurrentWaveIndex].SpawnInterval, 
                 false
            );
            UE_LOG(LogTemp, Warning, TEXT("RouteID:%d Scheduling Spawn %s in %.2f seconds"), 
                   RouteID, *NextEnemyID.ToString(), AssignedWaves[CurrentWaveIndex].SpawnInterval);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("RouteID:%d Delegate binding failed!"), RouteID);
        }

       
            UE_LOG(LogTemp, Warning,TEXT("RouteID:%d YamadaRyou4"), RouteID);
            UE_LOG(LogTemp, Warning, TEXT("RouteID:%d Scheduling Spawn %s in %.2f seconds"),RouteID, 
                   *NextEnemyID.ToString(), AssignedWaves[CurrentWaveIndex].SpawnInterval);
        
    }
    else
    {
        OnWaveSpawnComplete();
    }
}

void APVZ3DEnemySpawnPoint::OnWaveSpawnComplete()
{
    UE_LOG(LogTemp, Warning, TEXT("Wave spawn complete for Route %d"), RouteID);
    SpawnCompleted=1;
}