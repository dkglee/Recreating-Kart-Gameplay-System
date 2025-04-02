#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LicenseUI.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLicenseEndNotified);

UCLASS()
class PROJECTR_API ULicenseUI : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnLicenseEndNotified OnLicenseEndNotified;
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY()
	FWidgetAnimationDynamicEvent OnAnimationEndNotified;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LicenseShowAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> License;

	UFUNCTION()
	void OnLicenseEnd();
};
