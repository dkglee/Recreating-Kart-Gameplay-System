#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownToStart.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API UCountDownToStart : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent OnCountEndNotified;
	
protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	uint8 CountDownNum = 3;
	
	UPROPERTY(BlueprintReadWrite, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	FString StartText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountDownText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CountDownAnimation;

	UFUNCTION()
	void OnCountDownAnimationEnd();

	void UpdateCountDownText();
};
