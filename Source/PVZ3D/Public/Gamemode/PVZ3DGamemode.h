// PVZ3DGamemode.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PVZ3DGamemode.generated.h"

UCLASS()
class PVZ3D_API APVZ3DGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APVZ3DGamemode();
    
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
    
	// 开始关卡
	UFUNCTION(BlueprintCallable)
	void StartLevel();
    
	// 暂停关卡
	UFUNCTION(BlueprintCallable)
	void PauseLevel();
    
	// 继续关卡
	UFUNCTION(BlueprintCallable)
	void ResumeLevel();
    
	// 完成关卡
	UFUNCTION(BlueprintCallable)
	void CompleteLevel();
    
protected:
	// 检查是否所有玩家都已准备好
};