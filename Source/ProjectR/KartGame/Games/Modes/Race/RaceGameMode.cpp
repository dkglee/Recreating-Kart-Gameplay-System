#include "RaceGameMode.h"

#include "FastLogger.h"
#include "RaceGameState.h"
#include "RacePlayerController.h"
#include "GameFramework/PlayerState.h"

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARaceGameMode::StartGame()
{
	for (int i = 0; i < GetGameState<ARaceGameState>()->PlayerArray.Num(); i++)
	{
		ARacePlayerController* PC = Cast<ARacePlayerController>(GetGameState<ARaceGameState>()->PlayerArray[i]->GetPlayerController());
		if (PC->HasAuthority())
		{
			PC->SpawnKartWithCheckPoint(i + 1);
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this,
			&ThisClass::SetReadyGame, 15, false);
}

void ARaceGameMode::SetReadyGame()
{
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	
	if (GS->GetRaceStatus() != ERaceStatus::Idle)
	{
		// TODO: 중도 참여 즉 개발환경에 대한 로직을 추가하면 된다.
		return;
	}
	
	GS->SetReadyForTheMatch();
}