#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ItemRaceGameMode.generated.h"

UCLASS()
class PROJECTR_API AItemRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	uint16 MaxCheckPoint = 0;
};
