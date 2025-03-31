#include "TrackInfo.h"

#include "Components/TextBlock.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void UTrackInfo::InitializeData()
{
	MaxTrackInfo->SetText(FText::FromString(
			FString::FromInt(GetWorld()->GetGameState<ARaceGameState>()->GetMaxLaps())));
}

void UTrackInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsInitialized)
	{
		if (!GetOwningPlayerState<ARiderPlayerState>())
		{
			return;
		}
		
		GetOwningPlayerState<ARiderPlayerState>()->
		OnGoNextLapNotified.AddDynamic(this, &ThisClass::UpdateCurrentTrack);

		IsInitialized = true;
		return;
	}
	
	if (GetWorld()->GetGameState<ARaceGameState>()->GetRaceStatus() == ERaceStatus::Playing)
	{
		const FTimespan TimeDifference =
			FDateTime::Now() - GetWorld()->GetGameState<ARaceGameState>()->GetRaceStartTime();

		const uint32 TotalMinutes = TimeDifference.GetTotalMinutes();
		const uint8 Seconds = TimeDifference.GetSeconds();
		const int Milliseconds = TimeDifference.GetFractionMicro() / 1000;

		CurrentTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%03d")
			, TotalMinutes, Seconds, Milliseconds)));
	}
}

void UTrackInfo::UpdateCurrentTrack(const uint8 CurrentTrack)
{
	CurrentTrackInfo->SetText(FText::FromString(
			FString::FromInt(CurrentTrack + 1)));
}
