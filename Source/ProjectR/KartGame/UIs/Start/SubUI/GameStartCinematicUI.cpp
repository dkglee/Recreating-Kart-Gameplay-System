#include "GameStartCinematicUI.h"

#include "MediaPlayer.h"

void UGameStartCinematicUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartCinematic->OpenSource(StartCinematicSource))
	{
		StartCinematic->Rewind();
	}
}

void UGameStartCinematicUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (StartCinematic->IsPlaying())
	{
		if (StartCinematic->GetDuration().GetTotalSeconds() - RemainTimeToStartAnimationWhenCinematicEnd
		<= StartCinematic->GetTime().GetTotalSeconds()
		&& !IsStartAnimationWhenCinematicEnd)
		{
			IsStartAnimationWhenCinematicEnd = true;

			PlayAnimation(CinematicEndNearAnimation);
		}
	}
}
