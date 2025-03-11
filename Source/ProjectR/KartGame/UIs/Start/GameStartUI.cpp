#include "GameStartUI.h"

#include "MediaPlayer.h"

void UGameStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartCinematic->OpenSource(StartCinematicSource))
	{
		StartCinematic->Rewind();
	}
}

void UGameStartUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
