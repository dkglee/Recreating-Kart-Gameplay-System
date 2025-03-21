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
}

void UTrackInfo::UpdateCurrentTrack(const uint8 CurrentTrack)
{
	CurrentTrackInfo->SetText(FText::FromString(
			FString::FromInt(CurrentTrack + 1)));
}
