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

	const FText Timer = FText::FromString(FString::Printf(TEXT("%02d:%02d:%03d")
			, TotalMinutes, Seconds, Milliseconds));
	
	Rank->SetText(FText::FromString(FString::FromInt(PS->GetRanking())));
	PlayerName->SetText(FText::FromString(PS->GetPlayerName()));
	RunningTime->SetText(PS->GetIsRaceEnd() ? Timer : FText::FromString(RetireComment));
}
