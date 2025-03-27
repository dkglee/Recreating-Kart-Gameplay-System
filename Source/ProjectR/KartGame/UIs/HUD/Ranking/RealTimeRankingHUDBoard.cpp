#include "RealTimeRankingHUDBoard.h"

#include "RealTimeRankingHUDItem.h"
#include "Components/Overlay.h"
#include "GameFramework/PlayerState.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void URealTimeRankingHUDBoard::InitializeData()
{
	for (TObjectPtr<APlayerState> PlayerState : GetWorld()->GetGameState<ARaceGameState>()->PlayerArray)
	{
		ARiderPlayerState* PS = Cast<ARiderPlayerState>(PlayerState);

		URealTimeRankingHUDItem* NewRankBoard = CreateWidget<URealTimeRankingHUDItem>(this, RankItemClass);
		RankListOverlay->AddChildToOverlay(NewRankBoard);
		
		NewRankBoard->AddToViewport();
		NewRankBoard->InitializeData(PS);

		RankListItem.Add(NewRankBoard);
	}
}