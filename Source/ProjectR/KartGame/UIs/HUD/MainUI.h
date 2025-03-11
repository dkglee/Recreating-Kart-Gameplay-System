#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UCountDownToStart;

UCLASS()
class PROJECTR_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountDownToStart> CountDownToStartWidget;
};
