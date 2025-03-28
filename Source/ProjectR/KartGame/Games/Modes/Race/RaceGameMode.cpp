#include "RaceGameMode.h"

#include "RaceGameState.h"
#include "RacePlayerController.h"
#include "GameFramework/PlayerState.h"

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARaceGameMode::StartGame()
{
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	
	if (GS->GetRaceStatus() != ERaceStatus::Idle)
	{
		// TODO: 중도 참여 즉 개발환경에 대한 로직을 추가하면 된다.
		return;
	}
	
	GS->SetReadyForTheMatch();

	for (int i = 0; i < GS->PlayerArray.Num(); i++)
	{
		ARacePlayerController* PC = Cast<ARacePlayerController>(
			GS->PlayerArray[i]->GetPlayerController());
		
		if (!PC)
		{
			return;
		}

		if (PC->HasAuthority())
		{
			PC->SetHUDToStart();
			PC->SpawnKartWithCheckPoint(i + 1);
		} else
		{
			PC->Client_SetHUDToStart();
		}
	}
}