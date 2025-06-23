// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTower.h"
#include "PVZ3DHealthComponent.h"
#include "PVZ3DWeaponComponent.h"
#include "Actor/PVZ3DWeapon.h"
#include "Camera/CameraComponent.h"
#include "AI/PVZ3DTowerController.h"
#include "PVZ3D/CoreTypes/PVZ3DTowerCoreTypes.h"


void APVZ3DTower::BeginPlay()
{
	Super::BeginPlay();
	FGenericTeamId InitialTeamID = FGenericTeamId(2);
	UpdateTowerImformation();
	InitialOrientation= GetActorRotation();

	UpdateTower();
	
}

APVZ3DTower::APVZ3DTower()
{
	// 创建塔的静态网格组件
	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMeshComponent"));
	TowerMeshComponent->SetupAttachment(RootComponent);
	//TowerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TowerMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//TowerMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	//TowerMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	HealthComponent = CreateDefaultSubobject<UPVZ3DHealthComponent>(TEXT("HealthComponent"));

	WeaponComponent = CreateDefaultSubobject<UPVZ3DWeaponComponent>("WeaponComponent");

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
}

void APVZ3DTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DTower::Attack()
{
	Super::Attack();
	
	WeaponComponent->StartFire();
	WeaponComponent->StopFire();
	
	UE_LOG(LogTemp, Warning, TEXT("Tower Attack"));
}

void APVZ3DTower::Interact()

{
	Super::Interact();
}

void APVZ3DTower::UpdateCurrentWeaponID()
{
	Super::UpdateCurrentWeaponID();
	if (CurrentWeaponID==0)
	{
		CurrentWeaponID=1;
	}
	else
	{
		CurrentWeaponID=0;
	}
}

void APVZ3DTower::SwitchTower()
{
	UpdateCurrentWeaponID();
}

void APVZ3DTower::UpdateTower()
{
	if(TowerDataTable)
	{
		FTowerBasicInfo* Row = TowerDataTable->FindRow<FTowerBasicInfo>(
			CurrentWeaponID, 
			TEXT("LookupTowerData"), 
			true
		);
		UE_LOG(LogTemp, Warning, TEXT("TRY UPDATE TOWER") );
		if (Row) {
			CurrentWeaponID= Row->WeaponID;
			AttackRange = Row->AttackRange;
			AggroValue = Row->AggroValue;
			TowerBehaviorTreeNow = Row->BehaviourTreeID;
			UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponID: %s"), *CurrentWeaponID.ToString());
			UE_LOG(LogTemp, Warning, TEXT("AttackRange: %f"), AttackRange);
			UE_LOG(LogTemp, Warning, TEXT("AggroValue: %d"), AggroValue);
			UE_LOG(LogTemp, Warning, TEXT("TowerBehaviorTreeNow: %s"), *TowerBehaviorTreeNow.ToString());
		} else {
			// 处理未找到行的情况
			UE_LOG(LogTemp, Error, TEXT("找不到的数据"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TowerDataTable is null"));
	}
	if(GetController())
	{
		Cast<APVZ3DTowerController>(GetController())->UpdateTowerControllerImformation();
		UE_LOG(LogTemp, Warning, TEXT("Update Tower Controller Information"));
	}

	
	//后面写	
}

void APVZ3DTower::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tower Clicked"));
	SwitchTower();
	UE_LOG(LogTemp, Warning,TEXT("CurrentTowerHealth,%f"),HealthComponent->GetCurrentHealth());
}

void APVZ3DTower::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
	}
}

FGenericTeamId APVZ3DTower::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type APVZ3DTower::GetTeamAttitudeTowards(const AActor& Other) const
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

void APVZ3DTower::UpdateTowerImformation()
{
	//从表中读取塔的信息(阵营,武器,模型),Land 2,Platform 3
	SetGenericTeamId(FGenericTeamId(3));

}
