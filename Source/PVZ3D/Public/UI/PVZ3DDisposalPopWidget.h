// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "PVZ3DDisposalPopWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FRemove_2 , bool)

DECLARE_MULTICAST_DELEGATE_OneParam(FNumChanged1 , int32)

UCLASS()
class PVZ3D_API UPVZ3DDisposalPopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FRemove_2 Remove_2;

	FNumChanged1 NumChanged1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UButton* Button_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite , Category = "UI" ,  meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UFUNCTION(BlueprintCallable)
	void OnButton_0Clicked();

	UFUNCTION(BlueprintCallable)
	void OnButton_1Clicked();

	UFUNCTION(BlueprintCallable)
	void OnMouseLeave_1();
	
protected:
	virtual void NativeOnInitialized() override;
};
