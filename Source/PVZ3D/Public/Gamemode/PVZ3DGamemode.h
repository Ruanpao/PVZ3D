// PVZ3DGamemode.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PVZ3DGamemode.generated.h"


class APVZ3DPlayer;


UCLASS()
class PVZ3D_API APVZ3DGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APVZ3DGamemode();
    
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION()
	void OnPlayerDied(APlayerController* PlayerController);
    
	// 延迟复活玩家
	void DelayedRestartPlayer(APlayerController* PlayerController);

    
private:
	// 存储要复活的控制器（弱引用避免循环引用）
	TWeakObjectPtr<APlayerController> PlayerControllerToRespawn;	// 检查是否所有玩家都已准备好
};