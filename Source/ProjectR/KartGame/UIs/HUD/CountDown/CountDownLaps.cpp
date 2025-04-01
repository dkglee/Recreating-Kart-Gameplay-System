#include "CountDownLaps.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void UCountDownLaps::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!IsInitialize)
	{
		if (!GetOwningPlayerState<ARiderPlayerState>())
		{
			return;
		}

		OnCountEndNotified.Clear();
		OnCountEndNotified.BindDynamic(this, &ThisClass::OnAnimationEnd);
	
		BindToAnimationFinished(CountDownLapsAnimation, OnCountEndNotified);
		GetOwningPlayerState<ARiderPlayerState>()->
			OnGoNextLapNotified.AddDynamic(this, &ThisClass::UpdateLaps);
		
		IsInitialize = true;
	}
}

void UCountDownLaps::UpdateLaps(const uint8 NewLaps)
{
	LapsOverlay->SetVisibility(ESlateVisibility::Visible);
	
	FString NewLapsInfo;
	switch (NewLaps + 1)
	{
	case 1:
		{
			NewLapsInfo = FString::Printf(TEXT("%d st"), NewLaps + 1);
			break;
		}
	case 2:
		{
			NewLapsInfo = FString::Printf(TEXT("%dnd"), NewLaps + 1);
			break;
		}
	case 3:
		{
			NewLapsInfo = FString::Printf(TEXT("%drd"), NewLaps + 1);
			break;
		}
	default:
		{
			NewLapsInfo = FString::Printf(TEXT("%dth"), NewLaps + 1);
			break;
		}
	}
	
	if (GetWorld()->GetGameState<ARaceGameState>()->GetMaxLaps() == NewLaps + 1)
	{
		NewLapsInfo = TEXT("Final");
	}
	
	LapsText->SetText(FText::FromString(NewLapsInfo));
	PlayAnimation(CountDownLapsAnimation);
}

void UCountDownLaps::OnAnimationEnd()
{
	LapsOverlay->SetVisibility(ESlateVisibility::Hidden);
}
