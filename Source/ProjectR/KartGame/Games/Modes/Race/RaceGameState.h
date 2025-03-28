#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "EnumUtil.h"
#include "GameFramework/GameStateBase.h"
#include "RaceGameState.generated.h"

enum class ERaceStatus;
class ACheckPoint;


UCLASS()
class PROJECTR_API ARaceGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ARaceGameState();

#pragma region GetSet
	GETTER_SETTER(ERaceStatus, RaceStatus)
	GETTER_SETTER(FDateTime, RaceStartTime)
	GETTER(FDateTime, RaceEndTime)
	GETTER(TObjectPtr<ACheckPoint>, StartPoint)
	FORCEINLINE uint16 GetMaxCheckPoint() const { return MaxCheckPoint; }
	FORCEINLINE uint16 GetMaxLaps() const { return MaxLaps; }
	FORCEINLINE TMap<FString, ACheckPoint*> GetCheckPointData() const { return CheckPointData; }
#pragma endregion

	void CountDownToFinish(const FDateTime& FinishTime);
	void SetReadyForTheMatch();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	uint8 MaxLaps = 3;
	
	uint16 MaxCheckPoint = 0;

	uint8 CurrentPlayerCount = 0;

	UPROPERTY()
	TMap<FString, ACheckPoint*> CheckPointData;

	UPROPERTY()
	TObjectPtr<ACheckPoint> StartPoint;

	void SortRank();

	UPROPERTY(Replicated)
	ERaceStatus RaceStatus = ERaceStatus::Idle;

	UPROPERTY(Replicated)
	FDateTime RaceStartTime;
	
	UPROPERTY(Replicated)
	FDateTime RaceEndTime;
};