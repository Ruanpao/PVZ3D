// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DHome.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Components/TextRenderComponent.h"		//测试血量
#include "PVZ3DHealthComponent.h"
#include "Gamemode/PVZ3DGamemode.h"
#include "Gamemode/PVZ3DGameState.h"
#include "Kismet/GameplayStatics.h"



void APVZ3DHome::BeginPlay()
{
	Super::BeginPlay();
	SetGenericTeamId(FGenericTeamId(6));

	Gamemode = UGameplayStatics::GetGameMode(GetWorld());

	HealthComponent->OnDeath.AddUObject(this, &APVZ3DHome::OnHomeDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &APVZ3DHome::OnHealthChanged);
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), GetHomeCurrentHealth())));

	APVZ3DGameState* GameState = Cast<APVZ3DGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->UpdateHomeHealth(GetHomeCurrentHealth());
		UE_LOG(LogTemp, Warning, TEXT("AND Gamestate tried APVZ3DHome::BeginPlay - Home Health: %.0f"), GetHomeCurrentHealth());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APVZ3DHome::BeginPlay - GameState is null!"));
	}

}

void APVZ3DHome::OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), CurrentHealth)));

	OnHomeHealthChanged.Broadcast(CurrentHealth);
}

float APVZ3DHome::GetHomeCurrentHealth()
{
	return HealthComponent->GetCurrentHealth();
}

APVZ3DHome::APVZ3DHome()
{
	HealthComponent = CreateDefaultSubobject<UPVZ3DHealthComponent>(TEXT("HealthComponent"));

	HealthTextComponent= CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));	//测试血量
	HealthTextComponent->SetupAttachment(GetRootComponent());

	HealthComponent->SetMaxHealth(HomeMaxHealth); // 设置初始血量
}

void APVZ3DHome::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DHome::Interact()
{
	Super::Interact();
}

void APVZ3DHome::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Ichigaya Arisa"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Home Health: ") + FString::SanitizeFloat(HealthComponent->GetCurrentHealth()));
	UE_LOG(LogTemp, Warning, TEXT("Arisa Ichigaya"));
	UE_LOG(LogTemp, Warning, TEXT("Home Health: %f"), HealthComponent->GetCurrentHealth());
}

void APVZ3DHome::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
	}
}

FGenericTeamId APVZ3DHome::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type APVZ3DHome::GetTeamAttitudeTowards(const AActor& Other) const
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

void APVZ3DHome::OnHomeDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Home has been destroyed!"));
    
	// 更新GameState中的血量
	APVZ3DGameState* GameState = Cast<APVZ3DGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState)
	{
		GameState->UpdateHomeHealth(0.f);
        
		// 触发关卡失败
		if (GameState->IsLevelInProgress())
		{
			GameState->ServerFailLevel();
		}
	}
    
	HomeDeathDelegate.Broadcast(); // 触发事件
}
