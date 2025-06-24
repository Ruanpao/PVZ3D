// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemyController.h"
#include "AI/PVZ3DEnemy.h"
#include "AI/PVZ3DEnemyPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"


APVZ3DEnemyController::APVZ3DEnemyController()
{
	EnemyAIPerception = CreateDefaultSubobject<UPVZ3DEnemyPerceptionComponent>(TEXT("EnemyAIPerception"));
	SetPerceptionComponent(*EnemyAIPerception);
}

ETeamAttitude::Type APVZ3DEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	//UE_LOG(LogTemp,Warning,TEXT("EnemyGetTeamAttitudeTowards:%s"),*Other.GetName());
	
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamId == GetGenericTeamId() || FriendlyTeamID.Contains(OtherTeamId))
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 1111"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemyController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"), OtherTeamId.GetId());
			return ETeamAttitude::Friendly;
		}
		else if (HostileTeamID.Contains(OtherTeamId))
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 2222"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemyController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"),OtherTeamId.GetId());
			return ETeamAttitude::Hostile;
		}
		else
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 3333"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemyController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"), OtherTeamId.GetId());
			return ETeamAttitude::Neutral;
		}
	}
	//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 0000"));
	return ETeamAttitude::Neutral;
}

void APVZ3DEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto AimActor = GetTargetActor();
	if (AimActor)
	{
		
	}
	SetFocus(AimActor);

	
}

void APVZ3DEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(InPawn);
	RunPVZ3DEnemyBehaviorTree(Enemy->EnemyBehaviorTreeID);
	if (Enemy)
	{
		FGenericTeamId EnemyTeamID = Enemy->GetGenericTeamId();
		SetGenericTeamId(EnemyTeamID);

		//按照表的数据设置敌人的行为树以及Controller阵营
		if(EnemyTeamID==FGenericTeamId(4))
		{
			FriendlyTeamID={FGenericTeamId(3),FGenericTeamId(4),FGenericTeamId(5)};
			HostileTeamID={FGenericTeamId(1),FGenericTeamId(2)};
			//RunBehaviorTree(Enemy->EnemyBehaviorTree1);
		}
		if(EnemyTeamID==FGenericTeamId(5))
		{
			FriendlyTeamID={FGenericTeamId(4),FGenericTeamId(5)};
			HostileTeamID={FGenericTeamId(1),FGenericTeamId(2),FGenericTeamId(3)};
			//RunBehaviorTree(Enemy->EnemyBehaviorTree2);
		}
	}

	
	
}

void APVZ3DEnemyController::RunPVZ3DEnemyBehaviorTree(int BehaviorTreeID)
{
	APVZ3DEnemy* Enemy = Cast<APVZ3DEnemy>(GetPawn());
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemyController::RunPVZ3DEnemyBehaviorTree: Enemy is null!"));
		return;
	}
	if(Enemy->EnemyBehaviorTreeID==1)
		RunBehaviorTree(Enemy->EnemyBehaviorTree1);
	else if(Enemy->EnemyBehaviorTreeID==2)
		RunBehaviorTree(Enemy->EnemyBehaviorTree2);
	else if(Enemy->EnemyBehaviorTreeID==3)
		RunBehaviorTree(Enemy->EnemyBehaviorTree3);
	else
		RunBehaviorTree(Enemy->EnemyBehaviorTree1);//默认行为树
}

AActor* APVZ3DEnemyController::GetTargetActor() const
{
	if(!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetKeyName));
}


