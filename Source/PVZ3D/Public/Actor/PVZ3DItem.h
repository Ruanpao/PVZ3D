// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PVZ3DItem.generated.h"

UCLASS()
class PVZ3D_API APVZ3DItem : public AActor
{
	GENERATED_BODY()
    
public:    
	APVZ3DItem();

	virtual void StartUse();
	virtual void StopUse();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	AActor* ItemOwner;

	AActor* GetItemOwner() const;
public:    
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;
	
};