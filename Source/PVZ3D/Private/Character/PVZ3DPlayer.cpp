// Fill out your copyright notice in the Description page of Project Settings.


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
}

void APVZ3DPlayer::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	HealthComponent->OnDeath.AddUObject(this, &APVZ3DPlayer::OnDeath);
	//InputComponent->BindAction(TEXT("Fire"), IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);
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

		if (WeaponComponent && PlayerInputComponent)
		{
			PlayerInputComponent->BindAction("Attack", IE_Pressed, WeaponComponent, &UPVZ3DWeaponComponent::StartFire);
			PlayerInputComponent->BindAction("Attack", IE_Released, WeaponComponent, &UPVZ3DWeaponComponent::StopFire);
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
	}
}

void APVZ3DPlayer::StopAttack() {
	if (AttackAnimMontage && bIsAttacking) {
		StopAnimMontage(AttackAnimMontage);
		bIsAttacking = false;
	}
}

void APVZ3DPlayer::Interact()
{
	Super::Interact();
}

void APVZ3DPlayer::StartRun()
{
	Super::StartRun();
}

void APVZ3DPlayer::StopRun()
{
	Super::StopRun();
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

	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);

	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}