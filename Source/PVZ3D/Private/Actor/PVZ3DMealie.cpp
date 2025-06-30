// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DMealie.h"

APVZ3DMealie::APVZ3DMealie()
{
	ItemMesh->SetCollisionProfileName("NoCollision");
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetNotifyRigidBodyCollision(false);

	BuffValue = HealAmount;
}

void APVZ3DMealie::ApplyBuff(UPVZ3DHealthComponent* HealthComponent)
{
	if (HealthComponent)
	{
		HealthComponent->HandleBuffApplied(HealAmount , HealDuration);
		
		Destroy(); // 销毁物品
	}
}

void APVZ3DMealie::StopUse()
{
	Super::StopUse();
    
	if (GetItemOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found item owner: %s"), *ItemOwner->GetName());
		UPVZ3DHealthComponent* HealthComponent =GetItemOwner()->FindComponentByClass<UPVZ3DHealthComponent>();
		ApplyBuff(HealthComponent); 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Not!!!!"));
	}
}