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
	FORCEINLINE FString GetCurrentKartCheckPoint() const { return CurrentKartCheckPoint; }
	FORCEINLINE uint16 GetCurrentLap() const { return CurrentLap; }
	FORCEINLINE void SetCheckPoint(const FString& CheckPointNum) { CurrentKartCheckPoint = CheckPointNum; }
	FORCEINLINE void GoNextLap() { CurrentLap += 1; CurrentKartCheckPoint = TEXT("0"); }
	uint16 GetCurrentMainCheckPoint() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated)
	FString CurrentKartCheckPoint = TEXT("0");

	UPROPERTY(Replicated)
	uint8 CurrentLap = 0;
};