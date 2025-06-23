// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PVZ3DAnimInstance.h"
#include "Character/PVZ3DPlayer.h"
#include"AI/PVZ3DEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPVZ3DAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PVZ3DPlayer= Cast<APVZ3DPlayer>(TryGetPawnOwner());
}

void UPVZ3DAnimInstance::UpdateAnim(float DeltaTime)
{
	if (PVZ3DPlayer==nullptr)
	{
		PVZ3DPlayer= Cast<APVZ3DPlayer>(TryGetPawnOwner());
	}
	else
	{
		FVector PlayerVelocity= PVZ3DPlayer->GetVelocity();
		
		Velocity = PlayerVelocity.Size2D();
		Direction = PVZ3DPlayer->GetMovementDirection();
		IsInAir = PVZ3DPlayer->GetCharacterMovement()->IsFalling();
		Acceleration =PVZ3DPlayer->GetCharacterMovement()->GetCurrentAcceleration().Size();
		if( Velocity > 0.1f )
		{
			IsMove =true;
		}
		else
		{
			IsMove =false;
		}
	}

	if (PVZ3DEnemy==nullptr)
	{
		PVZ3DEnemy= Cast<APVZ3DEnemy>(TryGetPawnOwner());
	}
	else
	{
		FVector PlayerVelocity= PVZ3DEnemy->GetVelocity();
		
		Velocity = PlayerVelocity.Size2D();
		Direction = PVZ3DEnemy->GetMovementDirection();
		IsInAir = PVZ3DEnemy->GetCharacterMovement()->IsFalling();
		Acceleration =PVZ3DEnemy->GetCharacterMovement()->GetCurrentAcceleration().Size();
		if( Velocity > 0.1f )
		{
			IsMove =true;
		}
		else
		{
			IsMove =false;
		}
	}
}
