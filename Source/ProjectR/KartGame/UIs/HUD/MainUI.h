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
	GETTER(class UItemInventory*, WBP_ItemInventory);
	GETTER(TObjectPtr<UCountDownToStart>, CountDownToStartWidget)
#pragma endregion
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountDownToStart> CountDownToStartWidget;
	UPROPERTY(meta = (BindWidget))
	class UDashBoardUI* WBP_DashBoardUI;
	UPROPERTY(meta = (BindWidget))
	class UItemInventory* WBP_ItemInventory;
};
