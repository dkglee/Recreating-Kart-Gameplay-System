// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationTextUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UNotificationTextUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetShieldTextVisible(bool value);
	void SetDraftTextVisible(bool value);

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DraftText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShieldText;
};
