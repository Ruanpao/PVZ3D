
#include "UI/PVZ3DPlayerDetailWidget.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UPVZ3DPlayerDetailWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(GetWorld())
	{
		if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if(UPVZ3DHealthComponent* HealthComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DHealthComponent>())
			{
				UpdateHealthUI(HealthComponent->GetCurrentHealth(),HealthComponent->GetMaxHealth(),HealthComponent->GetHealthPercent());
				HealthComponent->OnHealthChanged.AddUObject(this, &UPVZ3DPlayerDetailWidget::UpdateHealthUI);
			}
		}
	}
}

void UPVZ3DPlayerDetailWidget::UpdateHealthUI(float CurrentHealth, float MaxHealth, float HealthPercent)
{
	WCurrentHealth = CurrentHealth;
	
	WMaxHealth = MaxHealth;
	
	WHealthPercent = HealthPercent;
}

