// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationUI.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"

void UNotificationUI::SetWaterBombInteractionUIVisible(bool value)
{
	if (value)
	{
		WaterBombInteractionText->SetVisibility(ESlateVisibility::Visible);
		LeftButtonImage->SetVisibility(ESlateVisibility::Visible);
		RightButtonImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WaterBombInteractionText->SetVisibility(ESlateVisibility::Hidden);
		LeftButtonImage->SetVisibility(ESlateVisibility::Hidden);
		RightButtonImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNotificationUI::SetDraftVisibleUI(bool value)
{
	if (value)
	{
		DraftTextBorder->SetVisibility(ESlateVisibility::Visible);
		PlayDraftTextBorderAnimation();
	}
	else
	{
		DraftTextBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNotificationUI::SetDraftProgressBar(float value)
{
	DraftProgressBar->SetValue(value);
	if (value >= 1.0)
	{
		DraftProgressBar->SetSliderProgressColor(FLinearColor(1.0,0.855670,0.0));
	}
	else
	{
		DraftProgressBar->SetSliderProgressColor(FLinearColor(0.0,0.800153,1.0));
	}
}
