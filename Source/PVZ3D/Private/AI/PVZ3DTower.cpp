// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTower.h"
#include "PVZ3DHealthComponent.h"
#include "PVZ3DWeaponComponent.h"
#include "Actor/PVZ3DWeapon.h"
#include "Camera/CameraComponent.h"
#include "AI/PVZ3DTowerController.h"
#include "Component/PVZ3DInventoryComponent.h"
#include "PVZ3D/CoreTypes/PVZ3DTowerCoreTypes.h"
#include "Character/PVZ3DPlayer.h" // 假设玩家类名为 PVZ3DPlayer


void APVZ3DTower::BeginPlay()
{
	Super::BeginPlay();
	FGenericTeamId InitialTeamID = FGenericTeamId(2);
	TeamID= InitialTeamID;
	//UpdateTowerImformation();
	InitialOrientation= GetActorRotation();
	bIsAttacking = false;
	UpdateTower();
	HealthComponent->OnDeath.AddUObject(this, &APVZ3DTower::TowerDied);
	HealthComponent->OnHealthChanged.AddUObject(this, &APVZ3DTower::OnHealthChanged);
	if (InventoryComponent)
	{
		InventoryComponent->HoldedItem = FItemInInventory(1,CurrentWeaponID, 1); // 初始化持有物品
		InventoryComponent->UpdateHoldedSlot(0); // 设置第一个槽为当前持有
	}
	BuildTower(CurrentWeaponID);
	// 获取Player0控制器

	// for (int32 i = 1; i < InventoryComponent->SlotSize; i++)
	// {
	// 	// 跳过第一个槽位(索引0)
	// 	if (i == 0) continue;
 //        
	// 	// 添加物品到槽位
	// 	InventoryComponent->AddToInventory(FName("0011"), 1);
	// }
	TowerInventory = FindComponentByClass<UPVZ3DInventoryComponent>();

	Player = Cast<APVZ3DPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	//StartInventoryCheckTimer();

	//this->PlayerWeaponChanged.AddDynamic(Player, &APVZ3DPlayer::OnHoldedItemChanged);

}

APVZ3DTower::APVZ3DTower()
{
	// 创建塔的静态网格组件
	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMeshComponent"));
	TowerMeshComponent->SetupAttachment(RootComponent);
	//TowerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TowerMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//TowerMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	//TowerMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	HealthComponent = CreateDefaultSubobject<UPVZ3DHealthComponent>(TEXT("HealthComponent"));

	WeaponComponent = CreateDefaultSubobject<UPVZ3DWeaponComponent>("WeaponComponent");
	InventoryComponent = CreateDefaultSubobject<UPVZ3DInventoryComponent>(TEXT("TowerInventoryComponent"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	Tags.Add(FName("Tower"));

	
}

void APVZ3DTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DTower::Attack()
{
	if (AttackType == 1)
	{
		// 单次攻击逻辑（非激光）
		WeaponComponent->StartFire();
		// 立即停止，适用于单次攻击
		WeaponComponent->StopFire();
		UE_LOG(LogTemp, Warning, TEXT("Tower Single Attack"));
	}
	else if (AttackType == 2)
	{
		// 激光武器（长按持续攻击）
		//if (!bIsAttacking)
		//{
			// 开始攻击
			WeaponComponent->StartFire();
			bIsAttacking = true;
			UE_LOG(LogTemp, Warning, TEXT("Tower Laser Attack Started"));
	}
	
	
}
void APVZ3DTower::StopAttack()
{
	if (AttackType == 2 && bIsAttacking)
	{
		WeaponComponent->StopFire();
		bIsAttacking = false;
		UE_LOG(LogTemp, Warning, TEXT("Tower Laser Attack Stopped"));
	}
}
void APVZ3DTower::Interact(AActor* InstigatorActor)
{

}

void APVZ3DTower::ApplyStateFromWeapon(UPVZ3DWeaponComponent* WeaponComp)
{
	if (!WeaponComp) return;
    
	FTowerState State = WeaponComp->GetCarriedTowerState();
	HealthComponent->SetCurrentHealth(State.CurrentHealth);
	// 可扩展应用其他状态（如弹药）
	// WeaponComponent->SetCurrentAmmo(State.CurrentAmmo);

}

void APVZ3DTower::SaveStateToWeapon(UPVZ3DWeaponComponent* WeaponComp)
{
	if (!WeaponComp) return;
    
	FTowerState State;
	State.CurrentHealth = HealthComponent->GetCurrentHealth();
	// 可扩展保存其他状态（如弹药）
	// State.CurrentAmmo = WeaponComponent->GetCurrentAmmo();
    
	WeaponComp->SetCarriedTowerState(State);
}

void APVZ3DTower::UpdateCurrentWeaponID()
{
	Super::UpdateCurrentWeaponID();
}

void APVZ3DTower::SwitchTower()
{
	UpdateCurrentWeaponID();
}

void APVZ3DTower::UpdateTower()//根据CurrentWeaponID更新塔的属性
{
	if(TowerDataTable)
	{
		FTowerBasicInfo* Row = TowerDataTable->FindRow<FTowerBasicInfo>(
			CurrentWeaponID, 
			TEXT("UpdateTower LookupTowerData"), 
			true
		);
		UE_LOG(LogTemp, Warning, TEXT("TRY UPDATE TOWER,CuttentWeaponID:%s"), *CurrentWeaponID.ToString());
		if (Row) {
			CurrentWeaponID= Row->WeaponID;
			AttackRange = Row->AttackRange;
			AggroValue = Row->AggroValue;
			TowerBehaviorTreeNow = Row->BehaviourTreeID;
			TeamID= Row->TeamID;
			if(TowerBehaviorTreeNow==FName("2"))
				AttackType= 2; // 激光武器
			else if(TowerBehaviorTreeNow==FName("1"))
				AttackType= 1; // 单次攻击
			else
				AttackType= 0; // 默认攻击类型)

			CurrentLevel= Row->CurrentLevel;
			MaxLevel=Row->CurrentMaxLevel;
			
			SetGenericTeamId(TeamID);
			UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponID: %s"), *CurrentWeaponID.ToString());
			UE_LOG(LogTemp, Warning, TEXT("AttackRange: %f"), AttackRange);
			UE_LOG(LogTemp, Warning, TEXT("AggroValue: %d"), AggroValue);
			UE_LOG(LogTemp, Warning, TEXT("TowerBehaviorTreeNow: %s"), *TowerBehaviorTreeNow.ToString());
			UE_LOG(LogTemp, Warning, TEXT("TeamID:%d "),TeamID.GetId());
			UE_LOG(LogTemp,Warning,TEXT("IMAS AttackType: %d"), AttackType);
			//TeamID
		} else {
			// 处理未找到行的情况
			UE_LOG(LogTemp, Error, TEXT("找不到的数据"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TowerDataTable is null"));
	}
	if(GetController())
	{
		Cast<APVZ3DTowerController>(GetController())->UpdateTowerControllerImformation();
		UE_LOG(LogTemp, Warning, TEXT("Update Tower Controller Information"));
	}
	
	
	//在controller中更新阵营和行为树
	//需要一个东西更新WeaponID
}

void APVZ3DTower::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tower Clicked"));
	// SwitchTower();
	// UE_LOG(LogTemp, Warning,TEXT("CurrentTowerHealth,%f"),HealthComponent->GetCurrentHealth());
	// BuildTower(FName("0004")); // 这里可以传入一个实际的武器ID

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tower Clicked - 测试武器交换"));
	UE_LOG(LogTemp, Warning, TEXT("Tower Clicked, CurrentHealth: %f"), HealthComponent->GetCurrentHealth());
	// 测试武器交换功能
	//Player = Cast<APVZ3DPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("找到玩家，开始测试武器交换"));
		SwapWeaponsWithPlayer(Player);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("未找到玩家，无法测试武器交换"));
	}
	// if (InventoryComponent)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("===== 塔的物品栏内容 ====="));
	// 	for (int32 i = 0; i < InventoryComponent->SlotSize; i++)
	// 	{
	// 		FItemInInventory SlotItem = InventoryComponent->Slot[i];
	// 		if (SlotItem.ID != "0000" && SlotItem.Quantity > 0)
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("物品栏 %d - ID: %s, 数量: %d"), 
	// 				   i, *SlotItem.ID.ToString(), SlotItem.Quantity);
	// 		}
	// 		else
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("物品栏 %d - 空槽位"), i);
	// 		}
	// 	}
	// 	UE_LOG(LogTemp, Warning, TEXT("========================"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("塔没有库存组件，无法检查物品栏"));
	// }
	UE_LOG(LogTemp,Warning,TEXT("CurrentHealth: %f"), HealthComponent->GetCurrentHealth());
}

void APVZ3DTower::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
	}
}

FGenericTeamId APVZ3DTower::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type APVZ3DTower::GetTeamAttitudeTowards(const AActor& Other) const
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
void APVZ3DTower::OnHealthChanged(float CurrentHealth, float MaxHealth, float HealthPercent)
{
	OnTowerHealthChanged.Broadcast(CurrentHealth);
}


void APVZ3DTower::BuildTower(FName NewWeaponID)
{
	FItemInInventory WeaponItem;
	// 模拟从某个地方获取武器信息
	CurrentWeaponID=NewWeaponID;
	WeaponItem.ID = CurrentWeaponID;
	UE_LOG(LogTemp, Warning, TEXT("BUILDTOWER1: Weapon ID: %s"), *WeaponItem.ID.ToString());
	if (WeaponComponent)
	{
		WeaponComponent->SwitchWeapon(WeaponItem);
		UE_LOG(LogTemp, Warning, TEXT("BUILDTOWER2: Weapon ID: %s"), *WeaponItem.ID.ToString());
		UpdateTower();
	}
}

void APVZ3DTower::TowerDied()
{
	CurrentWeaponID=FName("0000");
	BuildTower(FName("0000")); // 重置武器ID
	UpdateTower();
	UE_LOG(LogTemp, Warning, TEXT("Tower Died, CurrentWeaponID reset to 0000"));
	UE_LOG(LogTemp, Warning, TEXT("TEAMID: %d"), TeamID.GetId());
	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());
}

void APVZ3DTower::SellTower()
{
	BuildTower(FName("0000"));
	
}


void APVZ3DTower::SwapWeaponsWithPlayer(APVZ3DPlayer* aPlayer)
{
	UPVZ3DInventoryComponent* PlayerInventory = aPlayer->InventoryComponent;
	UPVZ3DInventoryComponent* aTowerInventory = InventoryComponent;

	FItemInInventory PlayerHolded = PlayerInventory->HoldedItem;
	FItemInInventory TowerHolded = aTowerInventory->HoldedItem;

	// 保存玩家当前持有槽位索引
	int32 PlayerOriginalSlot = PlayerHolded.Index;

	PlayerInventory->RemoveFromInventory(PlayerHolded.Index, true, false);
	TowerInventory->RemoveFromInventory(TowerHolded.Index, true, false);
	PlayerInventory->AddToInventory(TowerHolded.ID, TowerHolded.Quantity);
	TowerInventory->AddToInventory(PlayerHolded.ID, PlayerHolded.Quantity);

	// 塔始终使用第一个物品栏
	TowerInventory->UpdateHoldedSlot(0); // 强制设置为第一个槽位

	// 玩家使用交换前的槽位
	if (PlayerOriginalSlot >= 0 && PlayerOriginalSlot < PlayerInventory->SlotSize)
	{
		PlayerInventory->UpdateHoldedSlot(PlayerOriginalSlot);
	}
	else
	{
		// 槽位无效时使用第一个槽位作为 fallback
		PlayerInventory->UpdateHoldedSlot(0);
	}
	CurrentWeaponID= PlayerHolded.ID; // 更新塔的当前武器ID
	BuildTower(CurrentWeaponID);

	Player->WeaponComponent->SwitchWeapon(TowerHolded); // 更新玩家的武器组件
	
	// 应用状态（如果玩家的物品不是空）
	if (PlayerHolded.ID != "0000")
	{
		ApplyStateFromWeapon(Player->GetWeaponComponent());
	}
	else
	{
		// 空武器时恢复满血
		HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());
	}
}


void APVZ3DTower::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    // 停止定时器
    StopInventoryCheckTimer();
}
bool APVZ3DTower::IsTowerValid_Implementation() const
{
	return HealthComponent && !HealthComponent->IsDead();
}

bool APVZ3DTower::HasWeapon_Implementation() const
{
	UE_LOG(LogTemp,Warning, TEXT("HasWeapon called, CurrentWeaponID: %s"), *CurrentWeaponID.ToString());
	return WeaponComponent && CurrentWeaponID != FName("0000");
}

bool APVZ3DTower::IsTowerBaseInMiddle_Implementation() const
{
	return bIsBaseInMiddle;
}

bool APVZ3DTower::IsWeaponMaxLevel_Implementation() const
{
    return  CurrentLevel>= MaxLevel; 
}

void APVZ3DTower::StartInventoryCheckTimer()
{
    if (GetWorld() && TowerInventory && Player)
    {
        // 设置每1秒调用一次CheckAndLogInventories函数
        GetWorld()->GetTimerManager().SetTimer(
            InventoryCheckTimer,
            this,
            &APVZ3DTower::CheckAndLogInventories,
            1.0f,  // 间隔1秒
            true    // 循环执行
        );
        UE_LOG(LogTemp, Warning, TEXT("库存检查定时器已启动"));
    }
}

void APVZ3DTower::StopInventoryCheckTimer()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(InventoryCheckTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(InventoryCheckTimer);
        UE_LOG(LogTemp, Warning, TEXT("库存检查定时器已停止"));
    }
}

void APVZ3DTower::CheckAndLogInventories()
{
    UE_LOG(LogTemp, Warning, TEXT("===== 库存检查开始 ====="));
    
    // 日志塔的库存
    LogTowerInventory();
    
    // 日志玩家的库存
    LogPlayerInventory();
    
    UE_LOG(LogTemp, Warning, TEXT("===== 库存检查结束 =====\n"));
}

void APVZ3DTower::LogTowerInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("--- 塔的库存系统 ---"));
    if (!TowerInventory)
    {
        UE_LOG(LogTemp, Error, TEXT("塔没有库存组件"));
        return;
    }
    
    for (int32 i = 0; i < TowerInventory->SlotSize; i++)
    {
        FItemInInventory Item = TowerInventory->Slot[i];
        if (Item.ID == "0000" && Item.Quantity == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("槽位 %d: 空"), i);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("槽位 %d: ID=%s, 数量=%d"), i, *Item.ID.ToString(), Item.Quantity);
        }
    }
	UE_LOG(LogTemp,Warning,TEXT("CURRENT HEALTH TOWER:%f"),HealthComponent->GetCurrentHealth());
}

void APVZ3DTower::LogPlayerInventory()
{
    UE_LOG(LogTemp, Warning, TEXT("--- 玩家的库存系统 ---"));
    if (!Player || !Player->InventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("无法获取玩家库存组件"));
        return;
    }
    
    UPVZ3DInventoryComponent* PlayerInventory = Player->InventoryComponent;
    for (int32 i = 0; i < PlayerInventory->SlotSize; i++)
    {
        FItemInInventory Item = PlayerInventory->Slot[i];
        if (Item.ID == "0000" && Item.Quantity == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("槽位 %d: 空"), i);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("槽位 %d: ID=%s, 数量=%d"), i, *Item.ID.ToString(), Item.Quantity);
        }
    }
    
    // 额外日志玩家当前持有物品
    FItemInInventory HoldedItem = PlayerInventory->HoldedItem;
    UE_LOG(LogTemp, Warning, TEXT("玩家当前持有: ID=%s, 数量=%d, 槽位=%d"), 
           *HoldedItem.ID.ToString(), HoldedItem.Quantity, HoldedItem.Index);
}