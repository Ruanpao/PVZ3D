// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PVZ3DWeaponComponent.h"
#include"Actor/PVZ3DWeapon.h"
#include"Component/PVZ3DInventoryComponent.h"
#include"GameFramework/Character.h"
#include"Character/PVZ3DPlayer.h"
#include "Kismet/GameplayStatics.h"
#include"AI/PVZ3DTower.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All , All);


UPVZ3DWeaponComponent::UPVZ3DWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_HoldedItemDataTable.DT_HoldedItemDataTable' "));
}


void UPVZ3DWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld())
	{
		if(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			
			if(UPVZ3DInventoryComponent* InventoryComponent = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->FindComponentByClass<UPVZ3DInventoryComponent>())
			{
				InventoryComponent->HoldedChanged.AddUObject(this, &UPVZ3DWeaponComponent::SwitchWeapon);
			}
		}
	}
}

void UPVZ3DWeaponComponent::SwitchWeapon(FItemInInventory HoldedItem)
{
	DestroyWeapon();
	
	if(FWeaponBasicInfo* FoundWeaponInfo = DataTable->FindRow<FWeaponBasicInfo>(HoldedItem.ID , ""))
	{
		if(!FoundWeaponInfo->WeaponClass || !GetWorld()) return;
		
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character)return;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner =Character;
		SpawnParams.Instigator =Character;

		CurrentWeapon =GetWorld()->SpawnActor<APVZ3DWeapon>(FoundWeaponInfo->WeaponClass,Character->GetMesh()->GetSocketTransform(WeaponAttachPointName),SpawnParams);
		
		if(!CurrentWeapon)  return;
	
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,false);
		CurrentWeapon->AttachToComponent(Character->GetMesh(),AttachmentRules,WeaponAttachPointName);
	}
}

void UPVZ3DWeaponComponent::DestroyWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
	}
}


void UPVZ3DWeaponComponent::StartFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();

	
	APVZ3DPlayer* Player = Cast<APVZ3DPlayer>(GetOwner());
	if (Player) {
		Player->StartAttack(); 
	}
}

void UPVZ3DWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();

	APVZ3DPlayer* Player = Cast<APVZ3DPlayer>(GetOwner());
	if (Player) {
		Player->StopAttack(); 
	}
}

void UPVZ3DWeaponComponent::Reload()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}
