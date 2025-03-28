// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationTextUI.h"

#include "Components/TextBlock.h"

void UNotificationTextUI::SetShieldTextVisible(bool value)
{
	if (value)
	{
		ShieldText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ShieldText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNotificationTextUI::SetDraftTextVisible(bool value)
{
	if (value)
	{
		DraftText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DraftText->SetVisibility(ESlateVisibility::Hidden);
	}
}
