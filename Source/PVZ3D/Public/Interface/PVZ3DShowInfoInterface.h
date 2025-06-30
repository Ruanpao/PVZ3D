// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PVZ3DShowInfoInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPVZ3DShowInfoInterface : public UInterface
{
	GENERATED_BODY()
};


class PVZ3D_API IPVZ3DShowInfoInterface
{
	GENERATED_BODY()

public:
	virtual void ShowInfo(FName P_ID, int32 P_Quantity) = 0;
};
