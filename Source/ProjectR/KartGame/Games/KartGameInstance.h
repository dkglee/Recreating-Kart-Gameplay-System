#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KartGameInstance.generated.h"

UCLASS()
class PROJECTR_API UKartGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void Init() override;

};
