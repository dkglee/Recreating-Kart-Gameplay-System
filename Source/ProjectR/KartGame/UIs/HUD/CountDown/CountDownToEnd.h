#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownToEnd.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEndNotified);

UCLASS()
class PROJECTR_API UCountDownToEnd : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnGameEndNotified OnGameEndNotified;

	void PlayCountDown();
	
protected:
	virtual void NativePreConstruct() override;

private:
	FWidgetAnimationDynamicEvent OnCountEndNotified;
	
	// TODO: 해당 로직은 추후 GameState로 이전되어 연동될 예정
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	uint8 MaxCountDown = 10;
	
	uint8 CountDownNum = 10;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountDownText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CountDownAnimation;

	UFUNCTION()
	void OnCountDownAnimationEnd();

	void UpdateCountDownText();
};
