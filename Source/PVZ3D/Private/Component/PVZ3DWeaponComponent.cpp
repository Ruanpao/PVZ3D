// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PVZ3DWeaponComponent.h"
#include"Actor/PVZ3DWeapon.h"
#include"GameFramework/Character.h"
#include"Character/PVZ3DPlayer.h"
#include"AI/PVZ3DTower.h"
#include"Actor/PVZ3DCherry.h"

UPVZ3DWeaponComponent::UPVZ3DWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UPVZ3DWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadWeapons();
	
}

void UPVZ3DWeaponComponent::LoadWeapons()
{
	if(!WeaponDataTable||!GetWorld()) return;

	for(auto Weapon:WeaponInstances)
	{
		if(Weapon) Weapon->Destroy();
	}
	WeaponInstances.Empty();

	//读表
	TArray<FWeapon*> AllWeaponRows;
	WeaponDataTable->GetAllRows<FWeapon>(TEXT("WeaponID"), AllWeaponRows);

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	for(const FWeapon* WeaponRow :AllWeaponRows)
	{
		if(WeaponRow &&WeaponRow->WeaponClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Character;
			SpawnParams.Instigator = Character;

			APVZ3DWeapon*NewWeapon =GetWorld()->SpawnActor<APVZ3DWeapon>
			(
				WeaponRow->WeaponClass,
				Character->GetMesh()->GetSocketTransform(WeaponAttachPointName),
				SpawnParams);

			if(NewWeapon)
			{
				FAttachmentTransformRules AttachmentRules(
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::KeepWorld,
					false);

				NewWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
				
				NewWeapon->SetActorHiddenInGame(true);//一开始隐藏
				NewWeapon->SetActorEnableCollision(false); 
				WeaponInstances.Add(NewWeapon);
				
			}
		}
	}
	if(WeaponInstances.Num()>0)
	{
		CurrentWeaponIndex=0;
		CurrentWeapon =WeaponInstances[0];
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetActorEnableCollision(true);
	}
}

void UPVZ3DWeaponComponent::SpawnWeapon(const FWeapon& WeaponData)
{
	DestroyWeapon();

	if(!WeaponData.WeaponClass || !GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner =Character;
	SpawnParams.Instigator =Character;
	
	CurrentWeapon =GetWorld()->SpawnActor<APVZ3DWeapon>(WeaponData.WeaponClass,Character->GetMesh()->GetSocketTransform(WeaponAttachPointName),SpawnParams);
	

	if(!CurrentWeapon)  return;
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(),AttachmentRules,WeaponAttachPointName);
	
}

void UPVZ3DWeaponComponent::SwitchWeapon()
{
	if(WeaponInstances.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough weapons to switch."));
		return;
	}

	if(CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false); 
	}
	
	CurrentWeaponIndex =(CurrentWeaponIndex +1)%WeaponInstances.Num();
	CurrentWeapon=WeaponInstances[CurrentWeaponIndex];

	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentWeapon->SetActorEnableCollision(true);
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(Character&&Character->GetMesh())
	{
		FTransform SocketTransform = Character->GetMesh()->GetSocketTransform(WeaponAttachPointName);
		CurrentWeapon->SetActorLocation(SocketTransform.GetLocation());
		CurrentWeapon->SetActorRotation(SocketTransform.GetRotation());
		
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, 
			false);
            
		CurrentWeapon->AttachToComponent(
			Character->GetMesh(),
			AttachmentRules,
			WeaponAttachPointName);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Switched to weapon index: %d"), CurrentWeaponIndex);
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
