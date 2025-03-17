#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UCountDownToStart;

UCLASS()
class PROJECTR_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
#pragma region GETTERSETTER
	GETTER(class UDashBoardUI*, WBP_DashBoardUI);
#pragma endregion
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountDownToStart> CountDownToStartWidget;
	UPROPERTY(meta = (BindWidget))
	class UDashBoardUI* WBP_DashBoardUI;
};
