#include "RealTimeRankingHUDBoard.h"

#include "RealTimeRankingHUDItem.h"
#include "Components/Overlay.h"
#include "GameFramework/PlayerState.h"
#include "KartGame/Games/Modes/RaceGameState.h"
#include "KartGame/Games/Modes/RiderPlayerState.h"

void URealTimeRankingHUDBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (TObjectPtr<APlayerState> PlayerState : GetWorld()->GetGameState<ARaceGameState>()->PlayerArray)
	{
		ARiderPlayerState* PS = Cast<ARiderPlayerState>(PlayerState);

		URealTimeRankingHUDItem* NewRankBoard = CreateWidget<URealTimeRankingHUDItem>(this, RankItemClass);
		NewRankBoard->InitializeData(PS);
		RankListOverlay->AddChildToOverlay(NewRankBoard);
		
		NewRankBoard->AddToViewport();

		RankListItem.Add(NewRankBoard);
	}
	
	GetWorld()->GetGameState<ARaceGameState>()->OnRankingChangeNotified.AddDynamic(this, &ThisClass::OnChangedRanking);
}

void URealTimeRankingHUDBoard::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URealTimeRankingHUDBoard::OnChangedRanking(TArray<APlayerState*> PlayerList)
{
	
}
