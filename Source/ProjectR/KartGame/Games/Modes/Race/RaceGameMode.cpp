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
	ARaceGameState* GS = GetGameState<ARaceGameState>();
	
	if (GS->GetRaceStatus() != ERaceStatus::Idle)
	{
		return;
	}
	
	GS->SetRaceStatus(ERaceStatus::Ready);

	for (int i = 0; i < GS->PlayerArray.Num(); i++)
	{
		ARacePlayerController* PC = Cast<ARacePlayerController>(
			GS->PlayerArray[i]->GetPlayerController());
		
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