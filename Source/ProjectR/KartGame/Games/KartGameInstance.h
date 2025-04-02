#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Engine/GameInstance.h"
#include "KartGameInstance.generated.h"

UCLASS()
class PROJECTR_API UKartGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint8, InGamePlayerCount)
	GETTER_SETTER(uint8, PingCount)
	
protected:
	virtual void Init() override;

private:
	UPROPERTY()
	uint8 InGamePlayerCount = 1;
	
	UPROPERTY()
	uint8 PingCount = 0;

	UFUNCTION()
	void ClearSteam();
};
