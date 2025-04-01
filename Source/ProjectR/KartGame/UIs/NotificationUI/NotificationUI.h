// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UNotificationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetWaterBombInteractionUIVisible(bool value);
	void SetDraftVisibleUI(bool value);
	void SetDraftProgressBar(float value);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDraftTextBorderAnimation();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WaterBombInteractionText;

	UPROPERTY(meta = (BindWidget))
	class UImage* LeftButtonImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* RightButtonImage;
	
	UPROPERTY(meta = (BindWidget))
	class URadialSlider* DraftProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UBorder* DraftTextBorder;
};
