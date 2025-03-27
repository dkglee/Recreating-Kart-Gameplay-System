#include "TrackInfo.h"

#include "Components/TextBlock.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void UTrackInfo::NativeConstruct()
{
	Super::NativeConstruct();

	MaxTrackInfo->SetText(FText::FromString(
			FString::FromInt(GetWorld()->GetGameState<ARaceGameState>()->GetMaxLaps())));

	GetOwningPlayerState<ARiderPlayerState>()->
		OnGoNextLapNotified.AddDynamic(this, &ThisClass::UpdateCurrentTrack);

	RaceGameState = GetWorld()->GetGameState<ARaceGameState>();
}

void UTrackInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RaceGameState->GetRaceStatus() == ERaceStatus::Playing)
	{
		const FTimespan TimeDifference =
			FDateTime::Now() - RaceGameState->GetRaceStartTime();

		const uint32 TotalMinutes = TimeDifference.GetTotalMinutes();
		const uint8 Seconds = TimeDifference.GetSeconds();
		const float Milliseconds = (TimeDifference.GetTotalSeconds() * 1000) - TimeDifference.GetTotalMilliseconds();

		CurrentTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%03d")
			, TotalMinutes, Seconds, FMath::TruncToInt(Milliseconds))));
	}
}

void UTrackInfo::UpdateCurrentTrack(const uint8 CurrentTrack)
{
	CurrentTrackInfo->SetText(FText::FromString(
			FString::FromInt(CurrentTrack + 1)));
}
