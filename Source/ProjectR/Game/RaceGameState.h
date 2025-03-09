#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaceGameState.generated.h"

UCLASS()
class PROJECTR_API ARaceGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	FORCEINLINE uint16 GetMaxCheckPoint() const { return MaxCheckPoint; }

protected:
	virtual void BeginPlay() override;
	
private:
	uint16 MaxCheckPoint = 0;
};
