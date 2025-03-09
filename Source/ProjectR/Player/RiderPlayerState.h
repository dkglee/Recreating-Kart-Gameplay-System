#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RiderPlayerState.generated.h"

UCLASS()
class PROJECTR_API ARiderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FORCEINLINE uint16 GetCurrentKartCheckPoint() const { return CurrentKartCheckPoint; }
	void SetCheckPoint(const uint16 CheckPointNum);
	FORCEINLINE void GoNextRap() { CurrentRap += 1; }
	
private:
	uint16 CurrentKartCheckPoint = 0;
	uint8 CurrentRap = 0;
};
