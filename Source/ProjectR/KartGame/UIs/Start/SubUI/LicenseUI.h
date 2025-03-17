#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LicenseUI.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API ULicenseUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LicenseShowAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> License;
};
