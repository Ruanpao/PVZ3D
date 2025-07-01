#include "Character/PVZ3DPlayerController.h"
#include "Character/PVZ3DPlayer.h"
#include "PVZ3DWeaponComponent.h"
#include "UI/PVZ3DPlayerHUD.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerController01,All,All);

APVZ3DPlayerController::APVZ3DPlayerController()
{
	FGenericTeamId InitialTeamID=FGenericTeamId(1);
	//SetGenericTeamId(InitialTeamID);
	
}

void APVZ3DPlayerController::BeginPlay()
{
	Super::BeginPlay();

	

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APVZ3DPlayerController::InitializeHUD, 0.1f, false);
}

void APVZ3DPlayerController::InitializeHUD()
{
	MyHUD = Cast<APVZ3DPlayerHUD>(GetHUD());

	MyPlayer = Cast<APVZ3DPlayer>(GetPawn());

	if(MyHUD && MyPlayer)
	{
		MyHUD->OnMouseSituationChanged.AddUObject(MyPlayer,&APVZ3DPlayer::UpdateMouseSituation);
	}

	SetupInputComponent();
}

void APVZ3DPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(InputComponent && MyHUD)
	{
		InputComponent->BindAction("Open/CloseInventoryInformation",IE_Pressed, this , &APVZ3DPlayerController::ToggleInventoryInformationVisibility);

		InputComponent->BindAction("Open/CloseShop",IE_Pressed,this, &APVZ3DPlayerController::ToggleShopVisibility);
	}
}

void APVZ3DPlayerController::ToggleInventoryInformationVisibility()
{
	if(MyHUD)
	{
		MyHUD->InventoryInformationVisibility();
	}
}

void APVZ3DPlayerController::ToggleShopVisibility()
{
	if(MyHUD)
	{
		MyHUD->ShopVisibility();
	}
}

