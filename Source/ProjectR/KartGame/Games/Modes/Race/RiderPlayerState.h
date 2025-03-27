#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerState.h"
#include "RiderPlayerState.generated.h"

class ACheckPoint;
class FLifetimeProperty;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoNextLapNotified, uint8, LapInfo);

UCLASS()
class PROJECTR_API ARiderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ARiderPlayerState();
	
	FORCEINLINE FString GetCurrentKartCheckPoint() const { return CurrentKartCheckPoint; }
	FORCEINLINE uint16 GetCurrentLap() const { return CurrentLap; }
	void SetCheckPoint(const FString& CheckPointNum);
	void GoNextLap();
	
	uint16 GetCurrentMainCheckPoint() const;
	TObjectPtr<ACheckPoint> GetNextNearCheckPoint() const;

	GETTER_SETTER(uint8, Ranking);
	GETTER_SETTER(FDateTime, RaceEndTime);

	FOnGoNextLapNotified OnGoNextLapNotified;

protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY()
	TArray<uint16> PlayerCurrentCheckPointPinList;
	
	UPROPERTY(Replicated)
	FString CurrentKartCheckPoint = TEXT("0");

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_CurrentLap)
	uint8 CurrentLap = 0;

	UPROPERTY(Replicated)
	uint8 Ranking = 0;
	
	UPROPERTY(Replicated)
	FDateTime RaceEndTime;

	UFUNCTION()
	void OnRep_CurrentLap();
};