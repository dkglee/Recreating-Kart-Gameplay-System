#include "RiderPlayerState.h"

#include "ProjectR/KartGame/Utils/CheckPointUtil.h"

uint16 ARiderPlayerState::GetCurrentMainCheckPoint() const
{
	TArray<uint16> CheckPointPinList;
	FCheckPointUtil::GetCheckPointPinInfo(CurrentKartCheckPoint, CheckPointPinList);

	return CheckPointPinList[0];
}
