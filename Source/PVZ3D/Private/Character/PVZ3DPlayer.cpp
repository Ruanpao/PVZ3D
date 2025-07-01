// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/PVZ3DPlayer.h"
#include "Camera/CameraComponent.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/PVZ3DHealthComponent.h"
#include "Component/PVZ3DWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Actor/PVZ3DPlayerSpawnPoint.h"
#include "Gamemode/PVZ3DGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/PVZ3DPlayerSpawnPoint.h"
#include "AI/PVZ3DTower.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "PVZ3DWeaponComponent.h"
#include "Interface/UPVZ3DTowerInterface.h"

DEFINE_LOG_CATEGORY_STATIC(PVZ3DPlayerLog, All, All);


APVZ3DPlayer::APVZ3DPlayer()
{
	// 创建弹簧臂组件
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	// 创建摄像机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// 创建健康组件
	HealthComponent = CreateDefaultSubobject<UPVZ3DHealthComponent>(TEXT("HealthComponent"));

	// 创建库存组件
	InventoryComponent = CreateDefaultSubobject<UPVZ3DInventoryComponent>(TEXT("InventoryComponent"));
	


	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	bUseControllerRotationYaw = true;

	TeamID=FGenericTeamId(1); // 默认队伍 ID
	SetGenericTeamId(TeamID);

	WeaponComponent = CreateDefaultSubobject<UPVZ3DWeaponComponent>("WeaponComponent");

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	
	bTowerHasWeapon = false;
	bTowerBaseInMiddle = false;
	bTowerWeaponMaxLevel = false;
	bIsNearTower = false;
	CurrentTower = nullptr;

	TowerClass = APVZ3DTower::StaticClass();

	Tags.Add(FName("Player"));

}

void APVZ3DPlayer::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	//HealthComponent->OnDeath.AddUObject(this, &APVZ3DPlayer::OnDeath);

	HealthComponent->OnDeath.AddUObject(this, &APVZ3DPlayer::OnDeathInitiated); 
	
	//InputComponent->BindAction(TEXT("Fire"), IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);

	// APVZ3DGamemode* GameMode = Cast<APVZ3DGamemode>(GetWorld()->GetAuthGameMode());
	// if (GameMode)
	// {
	// 	OnPlayerDied.AddDynamic(GameMode, &APVZ3DGamemode::OnPlayerDied);
	// 	UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 已绑定死亡委托到GameMode"));
	// }
	// else
	// {
	// 	UE_LOG(PVZ3DPlayerLog, Error, TEXT("Player: 找不到GameMode，无法绑定委托"));
	// }
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APVZ3DPlayerSpawnPoint::StaticClass(), SpawnPoints);
    
	if (SpawnPoints.Num() > 0)
	{
		APVZ3DPlayerSpawnPoint* TheSpawnPoint = Cast<APVZ3DPlayerSpawnPoint>(SpawnPoints[0]);
		if (TheSpawnPoint)
		{
			SpawnPointLocation = TheSpawnPoint->GetActorLocation();            
			UE_LOG(LogTemp, Warning, TEXT("APVZ3DGameState::BeginPlay - Home found and events bound!"));
		}
	}
	if (InventoryComponent && WeaponComponent)
	{
		// 正确绑定事件（注意函数指针语法）
		InventoryComponent->HoldedChanged.AddUObject(this, &APVZ3DPlayer::OnHoldedItemChanged);

		WeaponComponent->OnConsumed.AddUObject(InventoryComponent, &UPVZ3DInventoryComponent::RemoveFromInventory);
	}

	WeaponComponent->BindSwitchWeapon(InventoryComponent);
	
}

void APVZ3DPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &APVZ3DPlayer::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &APVZ3DPlayer::MoveRight);
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
        
		PlayerInputComponent->BindAxis("LookRight", this, &APVZ3DPlayer::LookRight);
		PlayerInputComponent->BindAxis("LookUp", this, &APVZ3DPlayer::LookUp);
        
		PlayerInputComponent->BindAction("ChangeViewpoint", IE_Pressed, this, &APVZ3DPlayer::ChangeViewpoint);

		PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APVZ3DPlayer::StartRun);
		PlayerInputComponent->BindAction("Run", IE_Released, this, &APVZ3DPlayer::StopRun);

		PlayerInputComponent->BindAction("SwitchToStack1" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack1);
		PlayerInputComponent->BindAction("SwitchToStack2" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack2);
		PlayerInputComponent->BindAction("SwitchToStack3" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack3);
		PlayerInputComponent->BindAction("SwitchToStack4" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack4);
		PlayerInputComponent->BindAction("SwitchToStack5" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack5);
		PlayerInputComponent->BindAction("SwitchToStack6" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack6);
		PlayerInputComponent->BindAction("SwitchToStack7" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack7);
		PlayerInputComponent->BindAction("SwitchToStack8" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack8);
		PlayerInputComponent->BindAction("SwitchToStack9" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack9);
		PlayerInputComponent->BindAction("SwitchToStack10" , IE_Pressed , this , &APVZ3DPlayer::SwitchToStack10);

		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APVZ3DPlayer::Interact);

		
		if (WeaponComponent && PlayerInputComponent)
		{
			PlayerInputComponent->BindAction("Attack", IE_Pressed, WeaponComponent, &UPVZ3DWeaponComponent::StartFire);
			PlayerInputComponent->BindAction("Attack", IE_Released, WeaponComponent, &UPVZ3DWeaponComponent::StopFire);
			PlayerInputComponent->BindAction("Reload", IE_Released, WeaponComponent, &UPVZ3DWeaponComponent::OnReload);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInputComponent is null in SetupPlayerInputComponent"));
	}
}

void APVZ3DPlayer::MoveForward(const float Val)
{
	MovementInput.Y = Val;
	UpdateMovement(); 
	
}

void APVZ3DPlayer::MoveRight(const float Val)
{
	MovementInput.X = Val;
	UpdateMovement(); 
	
}

void APVZ3DPlayer::LookUp(const float Val)
{
	float NewPitch=SpringArmComponent->GetComponentRotation().Pitch+Val;
	NewPitch=FMath::Clamp(NewPitch,-75.0f,80.0f);
	FRotator NewRotator(NewPitch,0,0);
	SpringArmComponent->SetRelativeRotation(NewRotator);
}

void APVZ3DPlayer::LookRight(const float Val)
{
	AddControllerYawInput(Val);
}

void APVZ3DPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (!AnimInstance->Montage_IsActive(AttackAnimMontage))
			{
				AnimInstance->Montage_Play(AttackAnimMontage, 1.0f,EMontagePlayReturnType::MontageLength, 0.1f,false);
			}
		}
	}
}


void APVZ3DPlayer::Attack()
{
	Super::Attack();
	

	if (AttackAnimMontage)
	{
		PlayAnimMontage(AttackAnimMontage);
	}
}

void APVZ3DPlayer::StartAttack() {
	if (AttackAnimMontage && !bIsAttacking) {
		PlayAnimMontage(AttackAnimMontage, 1.0f,NAME_None); 
		bIsAttacking = true;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void APVZ3DPlayer::StopAttack() {
	if (AttackAnimMontage && bIsAttacking) {
		StopAnimMontage(AttackAnimMontage);
		bIsAttacking = false;
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

UPVZ3DWeaponComponent* APVZ3DPlayer::GetWeaponComponent() const
{
		return WeaponComponent;
}

void APVZ3DPlayer::Interact()
{
	Super::Interact();
	CurrentTower = FindNearestTowerInRange();
	InteractingTower=CurrentTower;


	if (CurrentTower)
	{
		// bIsNearTower = true;
		// bTowerHasWeapon = CurrentTower->HasWeapon(); // 假设Tower有此函数
		// bTowerBaseInMiddle = CurrentTower->IsTowerBaseInMiddle();
		// bTowerWeaponMaxLevel = CurrentTower->IsWeaponMaxLevel();
	}
	else
	{
		bIsNearTower = false;
	}

	BroadcastTowerInfo(CurrentTower);
}
APVZ3DTower* APVZ3DPlayer::FindNearestTowerInRange()
{
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRange);
    
	// 创建碰撞查询参数
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
    
	// 创建碰撞查询选项
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 忽略玩家自身
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
    
	// 执行重叠检测（注意：无论是否有结果，都会填充Overlaps数组）
	GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		Sphere,
		QueryParams
	);

	UE_LOG(LogTemp, Warning, TEXT("Found %d overlapping actors"), Overlaps.Num());

	APVZ3DTower* NearestTower = nullptr;
	float MinDistance = FLT_MAX;

	// 遍历重叠结果，找到最近的塔
	for (const FOverlapResult& Overlap : Overlaps)
	{
		APVZ3DTower* Tower = Cast<APVZ3DTower>(Overlap.GetActor());
		if (Tower && Tower->IsValidLowLevel())
		{
			float Distance = FVector::Dist(GetActorLocation(), Tower->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestTower = Tower;
			}
		}
	}

	// 调试可视化
	if (GetWorld()->IsGameWorld())
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			InteractionRange,
			32,
			NearestTower ? FColor::Green : FColor::Red,
			false,
			1.0f
		);
	}

	return NearestTower;
}

void APVZ3DPlayer::BroadcastTowerInfo(APVZ3DTower* Tower)
{
	if (!Tower)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tower is null"));
		bIsNearTower = false;
		OnTowerInteraction.Broadcast(false, false, false, false, nullptr);
		return;
	}
    
	// 通过接口检查塔有效性
	if (!Tower || !Tower->GetClass()->ImplementsInterface(UPVZ3DTowerInterface::StaticClass()))
	{
		bIsNearTower = false;
		OnTowerInteraction.Broadcast(false, false, false, false, nullptr);
		return;
	}
    
	IPVZ3DTowerInterface* TowerInterface = Cast<IPVZ3DTowerInterface>(Tower);
	bTowerHasWeapon = IPVZ3DTowerInterface::Execute_HasWeapon(TowerInterface->_getUObject());
	bTowerBaseInMiddle = IPVZ3DTowerInterface::Execute_IsTowerBaseInMiddle(TowerInterface->_getUObject());
	bTowerWeaponMaxLevel = IPVZ3DTowerInterface::Execute_IsWeaponMaxLevel(TowerInterface->_getUObject());
	bIsNearTower = true;
    
	OnTowerInteraction.Broadcast(bTowerHasWeapon, bTowerBaseInMiddle, bTowerWeaponMaxLevel, bIsNearTower, Tower);
	UE_LOG(LogTemp,Warning, TEXT("BroadcastTowerInfo: bTowerHasWeapon: %d, bTowerBaseInMiddle: %d, bTowerWeaponMaxLevel: %d, bIsNearTower: %d, Tower: %s"),
		bTowerHasWeapon, bTowerBaseInMiddle, bTowerWeaponMaxLevel, bIsNearTower, (Tower ? *Tower->GetName() : TEXT("None")));
}

void APVZ3DPlayer::StartRun()
{
	if(GetCharacterMovement()&&!bIsRunning)
	{
		bIsRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void APVZ3DPlayer::StopRun()
{
	if(GetCharacterMovement()&&bIsRunning)
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;	
	}
}

void APVZ3DPlayer::UpdateCurrentWeaponID()
{
	Super::UpdateCurrentWeaponID();
}

void APVZ3DPlayer::UpdateMovement()
{
	if (MovementInput.IsZero()) return;
	
	const FRotator CameraRot = CameraComponent->GetComponentRotation();
	const FRotator CameraYaw(0, CameraRot.Yaw, 0);
	
	const FVector ForwardDir = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::Y);
	const FVector MoveDirection = (ForwardDir * MovementInput.Y + RightDir * MovementInput.X).GetSafeNormal();
	
	AddMovementInput(MoveDirection, 1.0f);
	
	if (!MoveDirection.IsNearlyZero())
	{
		const FRotator TargetRotation = MoveDirection.Rotation();
		const FRotator NewRotation = FMath::RInterpTo(
			GetActorRotation(),
			TargetRotation,
			GetWorld()->GetDeltaSeconds(),
			0.1f 
		);
		
		SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
	}
}

void APVZ3DPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
	}
}

FGenericTeamId APVZ3DPlayer::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type APVZ3DPlayer::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamId == TeamID || FriendlyTeamIDs.Contains(OtherTeamId))
		{
			return ETeamAttitude::Friendly;
		}
		else if (HostileTeamIDs.Contains(OtherTeamId))
		{
			return ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}

float APVZ3DPlayer::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween =FMath::Acos(FVector::DotProduct(GetActorForwardVector(),VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(),VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween)*FMath::Sign(CrossProduct.Z);
}

void APVZ3DPlayer::ChangeViewpoint()
{
	if (ViewType==0)// 禁用鼠标输入
	{
		SpringArmComponent->TargetArmLength = 1000.0f;
		ViewType=1;
		UE_LOG(LogTemp, Warning, TEXT("ChangeViewpoint_Done,%f,ViewType:%d"), SpringArmComponent->TargetArmLength,ViewType);
		InputComponent->RemoveAxisBinding(TEXT("LookRight"));
		InputComponent->RemoveAxisBinding(TEXT("LookUp"));
		InputComponent->RemoveActionBinding(TEXT("Attack"), IE_Pressed);
		InputComponent->RemoveActionBinding(TEXT("Attack"), IE_Released);
		IsMouseInputDisabled = true;
		LastCameraRotation= SpringArmComponent->GetComponentRotation();
		SpringArmComponent->SetRelativeRotation(FRotator(-85, 0, 0));
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		Cast<APlayerController>(GetController())->bEnableClickEvents = true;
		InputComponent->BindAction(TEXT("PlayerMouseClick"), IE_Pressed, this, &APVZ3DPlayer::PlayerMouseClick);
		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());

		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
		SpringArmComponent->bInheritPitch = false;
		SpringArmComponent->bInheritYaw = false;
		SpringArmComponent->bInheritRoll = false;
		
		return;
	}
	if(ViewType==1)// 重新绑定鼠标输入
	{
		SpringArmComponent->TargetArmLength = 0.0f;
		ViewType=0;
		UE_LOG(LogTemp, Warning, TEXT("ChangeViewpoint_Done,%f,ViewType:%d"), SpringArmComponent->TargetArmLength,ViewType);
		SpringArmComponent->SetRelativeRotation(LastCameraRotation);
		InputComponent->BindAxis(TEXT("LookRight"), this, &APVZ3DPlayer::LookRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &APVZ3DPlayer::LookUp);
		InputComponent->RemoveActionBinding(TEXT("PlayerMouseClick"), IE_Pressed);
		InputComponent->BindAction("Attack", IE_Pressed, WeaponComponent, &UPVZ3DWeaponComponent::StartFire);
		InputComponent->BindAction("Attack", IE_Released, WeaponComponent, &UPVZ3DWeaponComponent::StopFire);
		IsMouseInputDisabled = false;
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
		Cast<APlayerController>(GetController())->bEnableClickEvents = false;
        Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());

		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = true;
		
		return;
	}
	
}

void APVZ3DPlayer::PlayerMouseClick()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerMouseClick_Done"));
	UE_LOG(LogTemp, Warning, TEXT("CurrentPlayerHealth,%f"),HealthComponent->GetCurrentHealth());
}


void APVZ3DPlayer::OnDeath()
{
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();

	if (WeaponComponent)
	{
		WeaponComponent->DestroyWeapon();
		WeaponComponent->DestroyComponent(); 
		WeaponComponent = nullptr;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController && PlayerController->IsValidLowLevel())
	{
		Controller->ChangeState(NAME_Spectating);
		OnPlayerDied.Broadcast(PlayerController); // 确保控制器有效时再广播
		UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player has died and will respawn in 5 seconds."));
	}
	else
	{
		UE_LOG(PVZ3DPlayerLog, Error, TEXT("Player::OnDeath - Controller is null or invalid"));
	}
    
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetLifeSpan(5.0f);
}

void APVZ3DPlayer::OnDeathInitiated()
{
    if (CurrentState != EPlayerState::Alive) return;
    UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 开始死亡流程"));

    CurrentState = EPlayerState::Dying;
    DeathLocation = GetActorLocation();
    DeathRotation = GetActorRotation();
    SavedHealth = HealthComponent ? HealthComponent->GetCurrentHealth() : 100.0f;
    
    // if (InventoryComponent) // 缓存物品（假设InventoryComponent支持）
    // {
    //     SavedInventory = InventoryComponent->GetCurrentInventory(); 
    // }

    if (DeathAnimMontage)
    {
        PlayAnimMontage(DeathAnimMontage);
    }

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if (WeaponComponent)
    {
        WeaponComponent->StopFire(); // 停止攻击
    }

    EnterSpectatorMode();

    StartRespawnTimer();
}

void APVZ3DPlayer::EnterSpectatorMode()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    LastCameraRotation = SpringArmComponent->GetComponentRotation();

    SpringArmComponent->TargetArmLength = 3000.0f;
    SpringArmComponent->SetRelativeRotation(FRotator(-85, 0, 0));

    PC->SetInputMode(FInputModeGameAndUI());
    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true;

    CurrentState = EPlayerState::Spectating;
    UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 进入旁观者模式"));
}

void APVZ3DPlayer::StartRespawnTimer()
{
    GetWorldTimerManager().SetTimer(
        RespawnTimerHandle,
        this,
        &APVZ3DPlayer::RespawnCharacter,
        RespawnDelay,
        false
    );
    UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 复活计时器启动（%fs后复活）"), RespawnDelay);
}


void APVZ3DPlayer::RespawnCharacter()
{
    if (CurrentState != EPlayerState::Spectating) return;
    UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 开始复活流程"));

    ExitSpectatorMode();

    CurrentState = EPlayerState::Respawning;
    SetActorLocationAndRotation(SpawnPointLocation, DeathRotation); // 恢复位置

    if (HealthComponent) HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());

    //  启用控制与碰撞
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    //  播放复活动画
    // if (RespawnAnimMontage)
    // {
    //     PlayAnimMontage(RespawnAnimMontage);
    // }

    GetWorldTimerManager().SetTimer(
        RespawnTimerHandle,
        [this]()
        {
            CurrentState = EPlayerState::Alive;
            UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 复活完成"));
        },
        1.5f, // 复活动画持续时间（根据实际调整）
        false
    );
	// PlayAnimMontage(AttackAnimMontage, 1.0f,NAME_None); 
	// StopAnimMontage(AttackAnimMontage);
		StopAnimMontage(DeathAnimMontage);
	
}

void APVZ3DPlayer::ExitSpectatorMode()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    SpringArmComponent->SetRelativeRotation(LastCameraRotation);
    SpringArmComponent->TargetArmLength = 0.0f; // 恢复第一人称视角

    PC->SetInputMode(FInputModeGameOnly());
    PC->bShowMouseCursor = false;
    PC->bEnableClickEvents = false;

    UE_LOG(PVZ3DPlayerLog, Warning, TEXT("Player: 退出旁观者模式"));
}

void APVZ3DPlayer::SwapWeaponsWithTower(APVZ3DTower* Tower)
{
	if (!Tower || !InventoryComponent) return;

	UPVZ3DInventoryComponent* PlayerInventory = InventoryComponent;
	UPVZ3DInventoryComponent* TowerInventory = Tower->InventoryComponent;

	FItemInInventory PlayerHoldedItem = PlayerInventory->HoldedItem;
	FItemInInventory TowerHoldedItem = TowerInventory->HoldedItem;

	// 交换逻辑与塔的实现对称
	PlayerInventory->RemoveFromInventory(PlayerHoldedItem.Index, true, false);
	TowerInventory->RemoveFromInventory(TowerHoldedItem.Index, true, false);

	PlayerInventory->AddToInventory(TowerHoldedItem.ID, TowerHoldedItem.Quantity);
	TowerInventory->AddToInventory(PlayerHoldedItem.ID, PlayerHoldedItem.Quantity);

	PlayerInventory->UpdateHoldedSlot(PlayerInventory->AnyEmptySlotAvailable() >= 0 ? PlayerInventory->AnyEmptySlotAvailable() : 0);
	TowerInventory->UpdateHoldedSlot(TowerInventory->AnyEmptySlotAvailable() >= 0 ? TowerInventory->AnyEmptySlotAvailable() : 0);

	// 更新玩家武器组件
	if (TowerHoldedItem.ID != "0000")
	{
		WeaponComponent->SwitchWeapon(TowerHoldedItem);
	}
}

void APVZ3DPlayer::OnHoldedItemChanged(FItemInInventory Item)
{
	if (WeaponComponent && Item.ID != "0000")
	{
		// 只更新玩家自身的武器组件
		WeaponComponent->SwitchWeapon(Item);
	}
}

void APVZ3DPlayer::SwitchToStack1()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(0);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[0]);
	}
}

void APVZ3DPlayer::SwitchToStack2()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(1);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[1]);
	}
}

void APVZ3DPlayer::SwitchToStack3()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(2);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[2]);
	}
}

void APVZ3DPlayer::SwitchToStack4()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(3);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[3]);
	}
}

void APVZ3DPlayer::SwitchToStack5()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(4);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[4]);
	}
}

void APVZ3DPlayer::SwitchToStack6()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(5);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[5]);
	}
}

void APVZ3DPlayer::SwitchToStack7()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(6);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[6]);
	}
}

void APVZ3DPlayer::SwitchToStack8()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(7);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[7]);
	}
}

void APVZ3DPlayer::SwitchToStack9()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(8);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[8]);
	}
}

void APVZ3DPlayer::SwitchToStack10()
{
	if(InventoryComponent)
	{
		InventoryComponent->UpdateHoldedSlot(9);
		WeaponComponent->SwitchWeapon(InventoryComponent->Slot[9]);
	}
}

