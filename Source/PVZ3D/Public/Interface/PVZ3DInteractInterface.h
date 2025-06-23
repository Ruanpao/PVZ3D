
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PVZ3DInteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPVZ3DInteractInterface : public UInterface
{
	GENERATED_BODY()
};


class PVZ3D_API IPVZ3DInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(AActor* InstigatorActor);
};
