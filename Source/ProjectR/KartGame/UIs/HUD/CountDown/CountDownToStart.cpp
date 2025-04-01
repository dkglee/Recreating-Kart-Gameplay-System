#include "CountDownToStart.h"

#include "FastLogger.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Components/TextBlock.h"

void UCountDownToStart::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateCountDownText();
}

void UCountDownToStart::OnCountDownAnimationStart()
{
	if (CountDownNum == 0)
	{
		PlaySound(GameStartSound);
		return;
	}
	PlaySound(CountDownSound);
}

void UCountDownToStart::OnCountDownAnimationEnd()
{
	if (CountDownNum != 0)
	{
		CountDownNum = FMath::Max(0, CountDownNum - 1);
		UpdateCountDownText();
		PlayAnimation(CountDownAnimation);
	} else
	{
		OnGameStartNotified.Broadcast();
	}
}

void UCountDownToStart::StartCountDown()
{
	OnCountStartNotified.Clear();
	OnCountStartNotified.BindDynamic(this, &ThisClass::OnCountDownAnimationStart);
	
	OnCountEndNotified.Clear();
	OnCountEndNotified.BindDynamic(this, &ThisClass::OnCountDownAnimationEnd);
	
	BindToAnimationStarted(CountDownAnimation, OnCountStartNotified);
	BindToAnimationFinished(CountDownAnimation, OnCountEndNotified);
	
	PlayAnimation(CountDownAnimation);
}

void UCountDownToStart::UpdateCountDownText()
{
	if (CountDownText)
	{
		if (CountDownNum == 0)
		{
			CountDownText->SetText(FText::FromString(StartText));
			return;
		}
		CountDownText->SetText(FText::FromString(FString::FromInt(CountDownNum)));
	}
}
