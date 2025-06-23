// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../CoreTypes/HealCoreTypes.h"
#include "PVZ3DHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged ,float,float,float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PVZ3D_API UPVZ3DHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPVZ3DHealthComponent();

	float GetCurrentHealth() const {return CurrentHealth;}

	float GetMaxHealth() const {return MaxHealth;}

	float GetHealthPercent() const {return CurrentHealth / MaxHealth;}
	
	UFUNCTION(BlueprintCallable)
	bool IsDead() const {return CurrentHealth <= 0.0f;}
	
	FOnDeath OnDeath;
	
	FOnHealthChanged OnHealthChanged;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
	FAutoHeal AutoHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal")
	FSteadyHeal SteadyHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal")
	FInstantHeal InstantHeal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
	float MaxHealth = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float CurrentHeal = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealUpdateTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
	int AutoHeal_TimeDelay_Count = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SteadyHeal")
	int SteadyHeal_Time_Count = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantHeal")
	int InstantHeal_TimeDelay_Count = 0;
	
	
	
	virtual void BeginPlay() override;

private:
	float CurrentHealth = 1000.0f;

	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
