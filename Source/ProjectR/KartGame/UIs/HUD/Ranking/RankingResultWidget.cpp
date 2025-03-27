#include "RankingResultWidget.h"

#include "Components/TextBlock.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void URankingResultWidget::InitializeData(ARiderPlayerState* PS)
{
	const FTimespan TimeDifference = PS->GetRaceEndTime() - 
		GetWorld()->GetGameState<ARaceGameState>()->GetRaceStartTime();
	
	const uint32 TotalMinutes = TimeDifference.GetTotalMinutes();
	const uint8 Seconds = TimeDifference.GetSeconds();
	const int Milliseconds = TimeDifference.GetFractionMicro() / 1000;
	
	Rank->SetText(FText::FromString(FString::FromInt(PS->GetRanking() + 1)));
	PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
	RunningTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%03d")
			, TotalMinutes, Seconds, Milliseconds)));
}
