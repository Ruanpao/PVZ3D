// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PVZ3DAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PVZ3D_API UPVZ3DAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation()  override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnim(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	class APVZ3DPlayer*PVZ3DPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	class APVZ3DEnemy*PVZ3DEnemy;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	float Velocity;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	float Direction;
	
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	bool IsInAir;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	bool IsMove;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Animation",meta=(AllowPrivateAccess="true"))
	float Acceleration;
};
