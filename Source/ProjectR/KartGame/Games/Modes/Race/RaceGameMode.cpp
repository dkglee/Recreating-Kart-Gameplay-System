#include "RaceGameMode.h"

#include "SessionUtil.h"
#include "OnlineSessionSettings.h"
#include "RaceGameState.h"
#include "RacePlayerController.h"
#include "GameFramework/PlayerState.h"

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
		for (const TObjectPtr<APlayerState> PlayerState : GetGameState<ARaceGameState>()->PlayerArray)
		{
			ARacePlayerController* PC = Cast<ARacePlayerController>(PlayerState->GetPlayerController());
			if (!PC)
			{
				return;
			}

			if (PC->IsLocalController())
			{
				PC->SetHUDToStart();
			} else
			{
				PC->Client_SetHUDToStart();
			}
		}
	}
}