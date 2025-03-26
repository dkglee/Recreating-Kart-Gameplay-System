#include "RaceGameMode.h"

#include "SessionUtil.h"
#include "OnlineSessionSettings.h"

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARaceGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	StartToPlayerCount += 1;

	if (StartToPlayerCount == FSessionUtil::GetCurrentSession()->SessionSettings.NumPublicConnections)
	{
		/**
		 * TODO: 여기에 게임 시작을 위한 스폰 위치 조정 작업과
		 * 게임 시작을 위한 트리거 설정을 진행한다.
		 */
	}
}
