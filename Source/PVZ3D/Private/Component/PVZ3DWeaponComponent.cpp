// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PVZ3DWeaponComponent.h"
#include"Actor/PVZ3DWeapon.h"
#include"Component/PVZ3DInventoryComponent.h"
#include"GameFramework/Character.h"
#include"Character/PVZ3DPlayer.h"
#include "Actor/PVZ3DSunflowerWeapon.h"
#include "Kismet/GameplayStatics.h"
#include"AI/PVZ3DTower.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All , All);


UPVZ3DWeaponComponent::UPVZ3DWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CarriedTowerState.CurrentHealth = 1000.0f;
	CarriedTowerState.CurrentAmmo = 0;
	

	DataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/MyAsset/DataTable/DT_HoldedItemDataTable.DT_HoldedItemDataTable'"));
}


void UPVZ3DWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if(APVZ3DPlayer* Owner = Cast<APVZ3DPlayer>(GetOwner()))
	{
		Owner->OnTowerInteraction.AddDynamic(this, &UPVZ3DWeaponComponent::Interact);
	}
	

}





void UPVZ3DWeaponComponent::SwitchWeapon(FItemInInventory HoldedItem)
{
	DestroyWeapon();

	CurrentHoldedItem = HoldedItem;
	
	if (HoldedItem.ID == FName("0000"))
	{
		// 如果 ID 为 0000，不创建新武器，直接返回
		CurrentWeapon = nullptr;
		CurrentItem = nullptr;
		return;
	}
	
	if(FWeaponBasicInfo* FoundHoldedItemInfo = DataTable->FindRow<FWeaponBasicInfo>(HoldedItem.ID , ""))
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character)return;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner =Character;
		SpawnParams.Instigator =Character;

		if((FoundHoldedItemInfo->ItemType == "Plant_Attack" || FoundHoldedItemInfo->ItemType == "Plant_Defense") && FoundHoldedItemInfo->WeaponClass)
		{
			CurrentWeapon = GetWorld()->SpawnActor<APVZ3DWeapon>(FoundHoldedItemInfo->WeaponClass,Character->GetMesh()->GetSocketTransform(WeaponAttachPointName),SpawnParams);
			
			if(!CurrentWeapon)  return;

			//赋值
			FString stringNum=FoundHoldedItemInfo->Clips.ToString();
			int32 CurrentBullets =FCString::Atoi(*stringNum);
			CurrentWeapon->SetAttribute(FoundHoldedItemInfo->Damage,FoundHoldedItemInfo->TraceMaxDistance,FoundHoldedItemInfo->ReloadTime,CurrentBullets);
			
			CurrentWeapon->OnReload.AddUObject(this, &UPVZ3DWeaponComponent::Reload);

			CurrentWeapon->Decrease.AddUObject(this, &UPVZ3DWeaponComponent::Decrease);

			CurrentWeaponClips = FoundHoldedItemInfo->Clips;
    
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,false);
			CurrentWeapon->AttachToComponent(Character->GetMesh(),AttachmentRules,WeaponAttachPointName);
		}
		else if(FoundHoldedItemInfo->ItemType == "Item_Damage" || FoundHoldedItemInfo->ItemType == "Item_Buff" && FoundHoldedItemInfo->ItemClass)
		{
			CurrentItem = GetWorld()->SpawnActor<APVZ3DItem>(FoundHoldedItemInfo->ItemClass,Character->GetMesh()->GetSocketTransform(WeaponAttachPointName),SpawnParams);

			if(!CurrentItem)  return;
    
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true);
			CurrentItem->AttachToComponent(Character->GetMesh(),AttachmentRules,WeaponAttachPointName);
		}
		
	}
}

void UPVZ3DWeaponComponent::DestroyWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		
	}

	if(CurrentItem)
	{
		CurrentItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentItem->Destroy();
		
	}
}

void UPVZ3DWeaponComponent::BindSwitchWeapon(UPVZ3DInventoryComponent* InventoryComponent)
{
	InventoryComponent->HoldedChanged.AddUObject(this, &UPVZ3DWeaponComponent::SwitchWeapon);
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

FTowerState UPVZ3DWeaponComponent::GetCarriedTowerState() const
{
	return CarriedTowerState;
}

void UPVZ3DWeaponComponent::SetCarriedTowerState(const FTowerState& NewState)
{
	CarriedTowerState = NewState;
}

void UPVZ3DWeaponComponent::Interact(bool IsFullTower,bool IsIntheMidLine, bool IsFullLevel , bool IsNearTower, APVZ3DTower* Tower)
{
	if(FWeaponBasicInfo* FoundHoldedItemInfo = DataTable->FindRow<FWeaponBasicInfo>(CurrentHoldedItem.ID , ""))
	{
		if(FoundHoldedItemInfo->ItemType == "Item_Damage" || FoundHoldedItemInfo->ItemType == "Item_Buff")
		{
			if(CurrentItem)
			{
				CurrentItem->StopUse();

				OnConsumed.Broadcast(CurrentHoldedItem.Index , false , true);
			}
			
			
		}
		else if((FoundHoldedItemInfo->ItemType == "None" || FoundHoldedItemInfo->ItemType == "Plant_Attack" || FoundHoldedItemInfo->ItemType == "Plant_Defense" ) && IsNearTower)
		{
			ButtonInteraction.Broadcast(false, IsNearTower, IsFullTower ,IsFullLevel , IsIntheMidLine , CurrentHoldedItem);


			UE_LOG(LogWeaponComponent, Warning , TEXT("Interact called with IsFullTower: %s, IsIntheMidLine: %s, IsFullLevel: %s, IsNearTower: %s, HoldedItem ID: %s"),
				IsFullTower ? TEXT("true") : TEXT("false"),
				IsIntheMidLine ? TEXT("true") : TEXT("false"),
				IsFullLevel ? TEXT("true") : TEXT("false"),
				IsNearTower ? TEXT("true") : TEXT("false"),
				*CurrentHoldedItem.ID.ToString());
			
		}
	}
}

void UPVZ3DWeaponComponent::OnReload()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartReload();
		
		Reloading.Broadcast(CurrentWeaponClips);
	}
}

void UPVZ3DWeaponComponent::Decrease(int32 CurrentBullet)
{
	FText CurrentBulletText = FText::AsNumber(CurrentBullet);
	UE_LOG(LogWeaponComponent , Warning , TEXT("456 %d"),CurrentBullet);
	Decreasing.Broadcast(CurrentBulletText);
}

