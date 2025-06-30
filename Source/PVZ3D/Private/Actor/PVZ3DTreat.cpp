// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DTreat.h"

APVZ3DTreat::APVZ3DTreat()
{
	ItemMesh->SetCollisionProfileName("NoCollision");
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetNotifyRigidBodyCollision(false);

	BuffValue = InstantHealAmount;
}

void APVZ3DTreat::ApplyBuff(UPVZ3DHealthComponent* HealthComponent)
{
	if(HealthComponent)
	{
		HealthComponent->ApplyInstantHeal(InstantHealAmount , HealDelay);
	}
}

void APVZ3DTreat::StopUse()
{
	Super::StopUse();
    
	if (GetItemOwner())
	{
		UPVZ3DHealthComponent* HealthComponent = GetItemOwner()->FindComponentByClass<UPVZ3DHealthComponent>();
		ApplyBuff(HealthComponent);
	}
}