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
	UPROPERTY()
	uint8 MaxLaps = 3;
	
	UPROPERTY()
	uint16 MaxCheckPoint = 0;

	UPROPERTY()
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

#pragma region NetworkClock

public:
	virtual double GetServerWorldTimeSeconds() const override;
	
	
private:
	// 서버의 시간 (앞으로 모든 클라이언트들이 해당 시간을 동기화 할 거임)
	// 해당 기능이 필요한 이유는 네트워크 지연으로 인한 시간 불일치를 해결하여
	// Dead Reckoning과 같은 동기화 처리를 할 때 반드시 필요로 함
	// 즉, Replicated를 하면 나타나는 네트워크 지연을
	// 보상하기 위한 정확한 시간을 동기화하기 위함
	double ServerWorldTimeSeconds = 0.0f;
	
#pragma endregion
};