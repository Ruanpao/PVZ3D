// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DEnemy.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Components/TextRenderComponent.h"		//测试血量
#include "PVZ3D/CoreTypes/PVZ3DEnemyCoreTypes.h"
#include "AI/PVZ3DEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gamemode/PVZ3DGameState.h"


APVZ3DEnemy::APVZ3DEnemy()
{
	RouteID=-1;
	EnemyID= FName("0000");

	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp,Warning,TEXT("Enemy : %s"),*GetName());
	FGenericTeamId InitialTeamID = FGenericTeamId(4); // 假设初始队伍 ID 为 4
	SetGenericTeamId(InitialTeamID);
	UE_LOG(LogTemp, Warning, TEXT("APVZ3DEnemy TeamID: %d"), GetGenericTeamId().GetId());

	HealthComponent = CreateDefaultSubobject<UPVZ3DHealthComponent>(TEXT("HealthComponent"));

	HealthTextComponent= CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));	//测试血量
	HealthTextComponent->SetupAttachment(GetRootComponent());

	Tags.Add(FName("Enemy"));
	
	//BehaviorTreeComponent=CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	//BlackboardComponent=CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void APVZ3DEnemy::BeginPlay()
{
	Super::BeginPlay();

	RouteManager= Cast<APVZ3DRouteManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APVZ3DRouteManager::StaticClass()));
	if(RouteManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("RouteManager found!"));
		UE_LOG(LogTemp, Warning, TEXT("RouteManager: %d"), RouteManager->GetRouteNodesByID(1)[0]->OrderIndex);

	}
	CurrentRouteNodes=RouteManager->GetRouteNodesByID(RouteID);
	if (CurrentRouteNodes[0]!=nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentRouteNodes: %d"), CurrentRouteNodes[0]->OrderIndex);
	}

	
	UpdateEnemyImformation();

	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetCurrentHealth(),HealthComponent->GetMaxHealth(),HealthComponent->GetHealthPercent());
	HealthComponent->OnDeath.AddUObject(this, &APVZ3DEnemy::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &APVZ3DEnemy::OnHealthChanged);

	APVZ3DGameState* GameState = Cast<APVZ3DGameState>(GetWorld()->GetGameState());
	if (GameState && HealthComponent)
	{
		// 绑定OnDeath事件到GameState的处理函数
		HealthComponent->OnDeath.AddUObject(GameState, &APVZ3DGameState::EnemyDead);
	}
}

void APVZ3DEnemy::OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));
}

void APVZ3DEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentNodeIndex: %d,CurrentRouteID:%d"), CurrentNodeIndex,RouteID);
	
}



void APVZ3DEnemy::SetRouteID(int NewRouteID)
{
	RouteID=NewRouteID;
}

void APVZ3DEnemy::MoveAlongRoute()
{
	
}

void APVZ3DEnemy::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
	}
}

FGenericTeamId APVZ3DEnemy::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type APVZ3DEnemy::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamId == TeamID || FriendlyTeamIDs.Contains(OtherTeamId))
		{
			return ETeamAttitude::Friendly;
		}
		else if (HostileTeamIDs.Contains(OtherTeamId))
		{
			return ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}


void APVZ3DEnemy::Attack()
{
	Super::Attack();
	
	PlayAnimMontage(AttackAnimMontage);
}

void APVZ3DEnemy::Interact()
{
	Super::Interact();
}

void APVZ3DEnemy::StartRun()
{
	Super::StartRun();
}

void APVZ3DEnemy::StopRun()
{
	Super::StopRun();
}

void APVZ3DEnemy::UpdateCurrentWeaponID()
{
	Super::UpdateCurrentWeaponID();
}

float APVZ3DEnemy::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween =FMath::Acos(FVector::DotProduct(GetActorForwardVector(),VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(),VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween)*FMath::Sign(CrossProduct.Z);
}

void APVZ3DEnemy::UpdateEnemyImformation()//通过EnemyID更新敌人信息
{
	if(EnemyDataTable)
	{
		FEnemyBasicInfo* Row = EnemyDataTable->FindRow<FEnemyBasicInfo>(
			EnemyID, 
			TEXT("UpdateEnemyImformation LookupEnemyData"), 
			true
		);
		UE_LOG(LogTemp, Warning, TEXT("PVZ3DEnemy UpdateEnemyImformation TRY UPDATE Enemy") );
		if (Row) {
			CurrentWeaponID= Row->WeaponID;
			AttackRange = Row->AttackRange;
			AggroValue = Row->AggroValue;
			EnemyBehaviorTreeID = Row->BehaviourTreeID;
			HealthComponent->SetMaxHealth(Row->MaxHealth);
			Vecolity = Row->Vecolity;
			GetCharacterMovement()->MaxWalkSpeed =Vecolity;
			AttackDamage=Row->Damage;
			AttackInterval=Row->AttackInterval;
			UE_LOG(LogTemp, Warning, TEXT("PVZ3DEnemy.cpp update row"));
			//UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponID: %s"), *CurrentWeaponID.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("AttackRange: %f"), AttackRange);
			//UE_LOG(LogTemp, Warning, TEXT("AggroValue: %d"), AggroValue);
			//UE_LOG(LogTemp, Warning, TEXT("TowerBehaviorTreeNow: %s"), *EnemyBehaviorTreeID.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("Vecolity: %d"), Vecolity);
			//UE_LOG(LogTemp, Warning, TEXT("MaxHealth: %f"), HealthComponent->GetMaxHealth());
			//TeamID
		} else {
			// 处理未找到行的情况
			UE_LOG(LogTemp, Error, TEXT("找不到的数据"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyDataTable is null"));
	}
	if(GetController())
	{
		Cast<APVZ3DEnemyController>(GetController())->UpdateEnemyControllerinformation();
		UE_LOG(LogTemp, Warning, TEXT("UpdateEnemyImformation: Update Enemy Controller Information"));
	}
}

void APVZ3DEnemy::UpdateEnemy()
{
}

void APVZ3DEnemy::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	SetActorEnableCollision(false);

	IsDead=1;
	
	SetLifeSpan(5.0f);
	//if (Controller)

}
