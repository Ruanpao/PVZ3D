// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PVZ3DTower.h"
#include "PVZ3DHealthComponent.h"
#include "PVZ3DWeaponComponent.h"
#include "Actor/PVZ3DWeapon.h"
#include "Camera/CameraComponent.h"
#include "AI/PVZ3DTowerController.h"
#include "PVZ3D/CoreTypes/PVZ3DTowerCoreTypes.h"




void APVZ3DTower::BeginPlay()
{
	Super::BeginPlay();
	FGenericTeamId InitialTeamID = FGenericTeamId(2);
	TeamID= InitialTeamID;
	//UpdateTowerImformation();
	InitialOrientation= GetActorRotation();

	UpdateTower();
	
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

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = false;


	TowerMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 允许和玩家重叠
	TowerMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	TowerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TowerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TowerMeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // 或 ECR_Overlap
	
}

void APVZ3DTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APVZ3DTower::Attack()
{
	Super::Attack();
	
	WeaponComponent->StartFire();
	WeaponComponent->StopFire();
	
	UE_LOG(LogTemp, Warning, TEXT("Tower Attack"));
}

void APVZ3DTower::Interact()

{
	Super::Interact();
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
			TEXT("LookupTowerData"), 
			true
		);
		UE_LOG(LogTemp, Warning, TEXT("TRY UPDATE TOWER") );
		if (Row) {
			CurrentWeaponID= Row->WeaponID;
			AttackRange = Row->AttackRange;
			AggroValue = Row->AggroValue;
			TowerBehaviorTreeNow = Row->BehaviourTreeID;
<<<<<<< Updated upstream
=======
			TeamID= Row->TeamID;
			if(Row->BehaviourTreeID==FName("2"))
				AttackType= 2; // 激光武器
			if(Row->BehaviourTreeID==FName("1"))
				AttackType= 1; // 单次攻击
			else
				AttackType= 0; // 默认攻击类型)

			CurrentLevel= Row->CurrentLevel;
			MaxLevel=Row->CurrentMaxLevel;
			
			SetGenericTeamId(TeamID);
>>>>>>> Stashed changes
			UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponID: %s"), *CurrentWeaponID.ToString());
			UE_LOG(LogTemp, Warning, TEXT("AttackRange: %f"), AttackRange);
			UE_LOG(LogTemp, Warning, TEXT("AggroValue: %d"), AggroValue);
			UE_LOG(LogTemp, Warning, TEXT("TowerBehaviorTreeNow: %s"), *TowerBehaviorTreeNow.ToString());
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
	SwitchTower();
	UE_LOG(LogTemp, Warning,TEXT("CurrentTowerHealth,%f"),HealthComponent->GetCurrentHealth());
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

void APVZ3DTower::UpdateTowerImformation()
{
	//从表中读取塔的信息(阵营,武器,模型),Land 2,Platform 3
	SetGenericTeamId(TeamID);

}
<<<<<<< Updated upstream
=======


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
>>>>>>> Stashed changes
