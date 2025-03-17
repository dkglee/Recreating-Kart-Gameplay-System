#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerState.h"
#include "RiderPlayerState.generated.h"

class ACheckPoint;

UCLASS()
class PROJECTR_API ARiderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ARiderPlayerState();
	
	FORCEINLINE FString GetCurrentKartCheckPoint() const { return CurrentKartCheckPoint; }
	FORCEINLINE uint16 GetCurrentLap() const { return CurrentLap; }
	void SetCheckPoint(const FString& CheckPointNum);
	FORCEINLINE void GoNextLap() { CurrentLap += 1; CurrentKartCheckPoint = TEXT("0"); }
	
	uint16 GetCurrentMainCheckPoint() const;
	TObjectPtr<ACheckPoint> GetNextNearCheckPoint() const;

	GETTER_SETTER(uint8, Ranking);

protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY()
	TArray<uint16> PlayerCurrentCheckPointPinList;
	
	UPROPERTY(Replicated)
	FString CurrentKartCheckPoint = TEXT("0");

	UPROPERTY(Replicated)
	uint8 CurrentLap = 0;

	UPROPERTY(Replicated)
	uint8 Ranking = 0;
};