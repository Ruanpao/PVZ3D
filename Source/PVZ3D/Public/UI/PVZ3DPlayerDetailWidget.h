#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PVZ3DPlayerDetailWidget.generated.h"

UCLASS()
class PVZ3D_API UPVZ3DPlayerDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
		UPROPERTY(EditDefaultsOnly, Category = "BindWidget")
	float WCurrentHealth = 1000.0f;

		UPROPERTY(EditDefaultsOnly, Category = "BindWidget")
	float WMaxHealth = 1000.0f;

		UPROPERTY(EditDefaultsOnly, Category = "BindWidget")
	float WHealthPercent = 1.0f;

	UFUNCTION(BlueprintCallable)
	float GetWCurrentHealth() const {return WCurrentHealth;}
	
	UFUNCTION(BlueprintCallable)
	float GetWMaxHealth() const {return WMaxHealth;}
	
	UFUNCTION(BlueprintCallable)
	float GetWHealthPercent() const {return WHealthPercent;}

protected:

	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealthUI(float CurrentHealth,float MaxHealth, float HealthPercent);
};
