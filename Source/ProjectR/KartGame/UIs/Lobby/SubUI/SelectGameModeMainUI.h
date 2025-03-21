#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectGameModeMainUI.generated.h"

class UWidgetSwitcher;

UCLASS()
class PROJECTR_API USelectGameModeMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> GameModeSwitcher;
};
