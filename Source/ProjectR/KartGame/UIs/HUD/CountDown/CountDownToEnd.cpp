#include "CountDownToEnd.h"

#include "Animation/WidgetAnimationEvents.h"
#include "Components/TextBlock.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RacePlayerController.h"

void UCountDownToEnd::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateCountDownText();
}

void UCountDownToEnd::OnCountDownAnimationEnd()
{
	ARaceGameState* GS = GetWorld()->GetGameState<ARaceGameState>();
	CountDownNum = FMath::Max(MaxCountDown -
		FMath::FloorToInt((FDateTime::Now() - GS->GetRaceEndTime()).GetTotalSeconds()), 0);

	UE_LOG(LogTemp, Display, TEXT("남은 카운트 : %d"), CountDownNum);
	
	if (CountDownNum != 0)
	{
		UpdateCountDownText();
		PlayAnimation(CountDownAnimation);
	} else
	{
		OnGameEndNotified.Broadcast();
	}
}

void UCountDownToEnd::PlayCountDown()
{
	SetVisibility(ESlateVisibility::Visible);
	OnCountEndNotified.Clear();
	OnCountEndNotified.BindDynamic(this, &ThisClass::OnCountDownAnimationEnd);
	BindToAnimationFinished(CountDownAnimation, OnCountEndNotified);
	PlayAnimation(CountDownAnimation);
}

void UCountDownToEnd::UpdateCountDownText()
{
	CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
}