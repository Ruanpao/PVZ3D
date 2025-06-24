// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTowerController.h"
#include "AI/PVZ3DTower.h"
#include "AI/PVZ3DTowerPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

APVZ3DTowerController::APVZ3DTowerController()
{
	TowerAIPerception = CreateDefaultSubobject<UPVZ3DTowerPerceptionComponent>(TEXT("TowerAIPerception"));
	SetPerceptionComponent(*TowerAIPerception);
}

ETeamAttitude::Type APVZ3DTowerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);

	//UE_LOG(LogTemp,Warning,TEXT("TowerGetTeamAttitudeTowards:%s"),*Other.GetName());
	
	
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamId == GetGenericTeamId() || FriendlyTeamID.Contains(OtherTeamId))
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 1111"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DTowerController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"), OtherTeamId.GetId());
			return ETeamAttitude::Friendly;
		}
		else if (HostileTeamID.Contains(OtherTeamId))
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 2222"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DTowerController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"),OtherTeamId.GetId());
			return ETeamAttitude::Hostile;
		}
		else
		{
			//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 3333"));
			//UE_LOG(LogTemp, Warning, TEXT("APVZ3DTowerController TeamID: %d"), GetGenericTeamId().GetId());
			//UE_LOG(LogTemp, Warning, TEXT("OtherTeamID: %d"), OtherTeamId.GetId());
			return ETeamAttitude::Neutral;
		}
	}
	//UE_LOG(LogTemp,Warning,TEXT("GetTeamAttitudeTowards: 0000"));
	return ETeamAttitude::Neutral;
}

void APVZ3DTowerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APVZ3DTowerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UpdateTowerControllerImformation();
}

void APVZ3DTowerController::UpdateTowerControllerImformation()
{
	APVZ3DTower* Tower = Cast<APVZ3DTower>(GetPawn());
	if (Tower)
	{
		Tower->UpdateTowerImformation();
		//根据表的数据设置塔的行为树以及Controller阵营
		FGenericTeamId TowerTeamID = Tower->GetGenericTeamId();
		SetGenericTeamId(TowerTeamID);

		//Land 2 Platform 3
		if(Tower->TowerBehaviorTreeNow==FName("1"))
		{
			RunBehaviorTree(Tower->TowerBehaviorTree1);
			UE_LOG(LogTemp,Warning,TEXT("TowerController使用行为树1"));
		}
		else if(Tower->TowerBehaviorTreeNow==FName("2"))
		{
			RunBehaviorTree(Tower->TowerBehaviorTree2);
			UE_LOG(LogTemp,Warning,TEXT("TowerController使用行为树2"));
		}
		else if(Tower->TowerBehaviorTreeNow==FName("3"))
		{
			RunBehaviorTree(Tower->TowerBehaviorTree3);
			UE_LOG(LogTemp,Warning,TEXT("TowerController使用行为树3"));
		}
		else if(Tower->TowerBehaviorTreeNow==FName("4"))
		{
			RunBehaviorTree(Tower->TowerBehaviorTree3);
			UE_LOG(LogTemp,Warning,TEXT("TowerController使用行为树4"));}
		else
		{
			//默认行为树
			UE_LOG(LogTemp,Warning,TEXT("TowerController没有找到对应的行为树"));
			RunBehaviorTree(Tower->TowerBehaviorTree1);
		}
	}
}

AActor* APVZ3DTowerController::GetTargetActor() const
{
	if(!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetKeyName));
}
