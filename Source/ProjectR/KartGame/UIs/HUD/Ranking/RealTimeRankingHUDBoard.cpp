#include "RealTimeRankingHUDBoard.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "KartGame/Games/Modes/RaceGameState.h"
#include "KartGame/Games/Modes/RiderPlayerState.h"

void URealTimeRankingHUDBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// int RankData = 0;
	// for (const auto PlayerState : GetWorld()->GetGameState<ARaceGameState>()->PlayerArray)
	// {
	// 	ARiderPlayerState* PS = Cast<ARiderPlayerState>(PlayerState);
	// 	if (!PS)
	// 	{
	// 		break;
	// 	}
	// 	
	// 	RankData += 1;
	// 	RankViewData.Add(PS, PS->GetRanking());
	// }
	//
	// for (TTuple<APlayerState*, uint8> ViewData : RankViewData)
	// {
	// 	UTextBlock* NewBlock = CreateWidget<UTextBlock>(this, UTextBlock::StaticClass());
	// 	NewBlock->SetText(FText::FromString(ViewData.Key->GetName()));
	// 	RankingListView.Add(ViewData.Key, NewBlock);
	// }
	
	GetWorld()->GetGameState<ARaceGameState>()->OnRankingChangeNotified.AddDynamic(this, &ThisClass::OnChangedRanking);
}

void URealTimeRankingHUDBoard::OnChangedRanking(TArray<APlayerState*> PlayerList)
{
}
