
#include "PVZ3DHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "../../CoreTypes/HealCoreTypes.h"
#include "Engine/World.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent,All,All);

UPVZ3DHealthComponent::UPVZ3DHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPVZ3DHealthComponent::ResetHealth()
{
	CurrentHealth = MaxHealth; 
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, 1.0f); 
}


// Called when the game starts
void UPVZ3DHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth,  GetHealthPercent());

	AActor* ComponentOwner = GetOwner();

	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UPVZ3DHealthComponent::OnTakeAnyDamage);
	}
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UPVZ3DHealthComponent::HealUpdate, HealUpdateTime, true);
	}
}

void  UPVZ3DHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f || IsDead() || !GetWorld()) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetHealthPercent());

	UE_LOG(LogHealthComponent, Display, TEXT("Character is dead!"));

	if (IsDead())
	{
		OnDeath.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle); // 停止治疗定时器
		return; 
	}
	else if(AutoHeal.IsAutoHeal)
	{
		AutoHeal_TimeDelay_Count = 0;
	}
	
}

void  UPVZ3DHealthComponent::HealUpdate()
{
	if(AutoHeal.IsAutoHeal && AutoHeal_TimeDelay_Count < AutoHeal.AutoHeal_TimeDelay/HealUpdateTime)
	{
		AutoHeal_TimeDelay_Count += 1;
	}
	else if(AutoHeal.IsAutoHeal && AutoHeal_TimeDelay_Count == AutoHeal.AutoHeal_TimeDelay/HealUpdateTime)
	{
		CurrentHeal += AutoHeal.AutoHeal_Amount;
	}
	
	if(SteadyHeal.IsSteadyHeal && SteadyHeal_Time_Count < SteadyHeal.SteadyHeal_Time/HealUpdateTime)
	{
		CurrentHeal += SteadyHeal.SteadyHeal_Amount;
			
		SteadyHeal_Time_Count += 1;
	}
	else if(SteadyHeal.IsSteadyHeal && SteadyHeal_Time_Count > SteadyHeal.SteadyHeal_Time/HealUpdateTime)
	{
		SteadyHeal_Time_Count = 0;
		
		SteadyHeal.IsSteadyHeal = false;
	}
	
	if(InstantHeal.IsInstantHeal && InstantHeal_TimeDelay_Count < InstantHeal.InstantHeal_TimeDelay/HealUpdateTime)
	{
		InstantHeal_TimeDelay_Count += 1;
	}
	else if(InstantHeal.IsInstantHeal && InstantHeal_TimeDelay_Count == InstantHeal.InstantHeal_TimeDelay/HealUpdateTime)
	{
		CurrentHeal += InstantHeal.InstantHeal_Amount;
		
		InstantHeal_TimeDelay_Count = 0;
		
		InstantHeal.IsInstantHeal = false;
	}
	
	if(CurrentHealth < MaxHealth && CurrentHeal > 0.0f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth + CurrentHeal, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetHealthPercent());
	}
	
	CurrentHeal = 0.0f;
}

void UPVZ3DHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPVZ3DHealthComponent::HandleBuffApplied(float Amount, float Duration)
{
    
	if (Duration > 0.0f)
	{
		// 持续治疗
		SteadyHeal.IsSteadyHeal = true;
		SteadyHeal.SteadyHeal_Amount = Amount;
		SteadyHeal.SteadyHeal_Time = Duration;
		SteadyHeal_Time_Count = 0;
	}
	else
	{
		ApplyInstantHeal(Amount, Duration);
	}
}

void UPVZ3DHealthComponent::ApplyInstantHeal(float Amount, float Delay)
{
	UE_LOG(LogHealthComponent, Warning, TEXT("Applying instant heal: Amount=%.1f, Delay=%.1f"), Amount, Delay);
    
	// 设置即时治疗
	InstantHeal.IsInstantHeal = true;
	InstantHeal.InstantHeal_Amount = Amount;
	InstantHeal.InstantHeal_TimeDelay = Delay;
	InstantHeal_TimeDelay_Count = 0; // 重置计时器
}
