#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownLaps.generated.h"

class UHorizontalBox;
class UTextBlock;

UCLASS()
class PROJECTR_API UCountDownLaps : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	FWidgetAnimationDynamicEvent OnCountEndNotified;

	UPROPERTY()
	bool IsInitialize = false;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LapsText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> LapsOverlay;
	
	UFUNCTION()
	void UpdateLaps(const uint8 NewLaps);
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CountDownLapsAnimation;

	UFUNCTION()
	void OnAnimationEnd();
};
