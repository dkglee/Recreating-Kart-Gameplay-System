#include "RiderPlayerState.h"

#include "ProjectR/Game/RaceGameState.h"


void ARiderPlayerState::SetCheckPoint(const uint16 CheckPointNum)
{
	// TODO: 최대 체크포인트 정보 가져올 시 0으로 초기화 해야함.
	const ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>();

	// 게임모드 없으면 우선 그냥 반환 처리
	if (!IsValid(GS))
	{
		return;
	}

	// 최대 체크포인트 없으면 반환처리
	if (CheckPointNum > GS->GetMaxCheckPoint())
	{
		return;
	}

	CurrentKartCheckPoint = CheckPointNum;
}
