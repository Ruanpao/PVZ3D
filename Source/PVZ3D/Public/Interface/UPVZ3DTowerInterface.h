// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPVZ3DTowerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPVZ3DTowerInterface : public UInterface
{
	GENERATED_BODY()
};

class IPVZ3DTowerInterface
{
	GENERATED_BODY()
    
public:
	// 检查塔是否有效（可交互）
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsTowerValid() ;
    
	// 获取塔基础信息
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasWeapon() ;
    
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsTowerBaseInMiddle() const;
    
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWeaponMaxLevel() ;
};