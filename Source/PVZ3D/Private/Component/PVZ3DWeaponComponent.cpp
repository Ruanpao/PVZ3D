// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PVZ3DWeaponComponent.h"
#include"Actor/PVZ3DWeapon.h"
#include"GameFramework/Character.h"
#include"Character/PVZ3DPlayer.h"
#include"AI/PVZ3DTower.h"

UPVZ3DWeaponComponent::UPVZ3DWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UPVZ3DWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	
}

void UPVZ3DWeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)return;

	if (!WeaponClass) {
		UE_LOG(LogTemp, Error, TEXT("WeaponClass is null!"));
		return;
	}
	
	CurrentWeapon = GetWorld()->SpawnActor<APVZ3DWeapon>(WeaponClass);
	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
		return;
	}
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Character);

	UE_LOG(LogTemp, Warning, TEXT("Weapon attached to: %s"), *WeaponAttachPointName.ToString());
}

void UPVZ3DWeaponComponent::DestroyWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy(); 
		CurrentWeapon = nullptr;  
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
