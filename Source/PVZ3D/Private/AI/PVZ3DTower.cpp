// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTower.h"
#include "PVZ3DHealthComponent.h"




void APVZ3DTower::BeginPlay()
{
	Super::BeginPlay();
	FGenericTeamId InitialTeamID = FGenericTeamId(2);
	UpdateTowerImformation();
	InitialOrientation= GetActorRotation();
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
	
}

void APVZ3DTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DTower::Attack()
{
	Super::Attack();
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
	switch (CurrentWeaponID)
	{
		case 0:
			UE_LOG(LogTemp, Warning, TEXT("SwitchTower_Done,CurrentWeaponID:%d"), CurrentWeaponID);
			break;
		case 1:
			UE_LOG(LogTemp, Warning, TEXT("SwitchTower_Done,CurrentWeaponID:%d"), CurrentWeaponID);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("SwitchTower_Default"));
			break;
	}
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
