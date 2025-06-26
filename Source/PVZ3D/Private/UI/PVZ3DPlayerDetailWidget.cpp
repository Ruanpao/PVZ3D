
#include "UI/PVZ3DPlayerDetailWidget.h"
#include "Component/PVZ3DInventoryComponent.h"
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

			if(UPVZ3DInventoryComponent* InventoryComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DInventoryComponent>())
			{
				UpdateGoldUI(InventoryComponent->GetCurrentGold());
				InventoryComponent->OnGoldChanged.AddUObject(this, &UPVZ3DPlayerDetailWidget::UpdateGoldUI);
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

void UPVZ3DPlayerDetailWidget::UpdateGoldUI(int32 Gold)
{
	WGold = Gold;
}

