#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RiderPlayerState.generated.h"

class ACheckPoint;

UCLASS()
class PROJECTR_API ARiderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FORCEINLINE uint16 GetCurrentKartCheckPoint() const { return CurrentKartCheckPoint; }
	FORCEINLINE uint16 GetCurrentLap() const { return CurrentLap; }
	void SetCheckPoint(const uint16 CheckPointNum);
	FORCEINLINE void GoNextLap() { CurrentLap += 1; }
	
private:
	UPROPERTY()
	TObjectPtr<ACheckPoint> CurrentCheckPointPin;
	
	uint16 CurrentKartCheckPoint = 0;
	uint8 CurrentLap = 0;
};