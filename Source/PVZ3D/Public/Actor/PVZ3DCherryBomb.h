// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/PVZ3DItemDamage.h"
#include "PVZ3DCherryBomb.generated.h"

UCLASS()
class PVZ3D_API APVZ3DCherryBomb : public APVZ3DItemDamage
{
	GENERATED_BODY()
	
public:	
	APVZ3DCherryBomb();
	virtual void BeginPlay() override;

	// 开火函数（按压使用键）
	virtual void StartUse() override;
    
	// 停火函数（松手投掷）
	virtual void StopUse() override;
    
	// 投掷炸弹
	virtual void MakeShot() override;
    
	// 计算抛物线轨迹
	virtual void CalculateProjectilePath() override;
    
	// 清除轨迹预览
	virtual void ClearProjectilePath() override;
    
	// 碰撞处理
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
	// 激活炸弹
	virtual void ActivateBomb(FVector Location, FVector Direction, AActor* NewWeaponOwner, AController* NewWeaponInstigator) override;
    
	// 停用炸弹
	virtual void DeactivateBomb() override;
	
};
