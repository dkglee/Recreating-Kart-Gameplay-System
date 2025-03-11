#include "RiderPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "ProjectR/KartGame/Utils/CheckPointUtil.h"

uint16 ARiderPlayerState::GetCurrentMainCheckPoint() const
{
	TArray<uint16> CheckPointPinList;
	FCheckPointUtil::GetCheckPointPinInfo(CurrentKartCheckPoint, CheckPointPinList);

	return CheckPointPinList[0];
}

void ARiderPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARiderPlayerState, CurrentLap);
	DOREPLIFETIME(ARiderPlayerState, CurrentKartCheckPoint);
}