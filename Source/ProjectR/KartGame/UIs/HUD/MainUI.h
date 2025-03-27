#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UDashBoardUI;
class UItemInventory;
class URealTimeRankingHUDBoard;
class UCountDownToStart;

UCLASS()
class PROJECTR_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData();
	
#pragma region GETTERSETTER
	GETTER(TObjectPtr<UDashBoardUI>, WBP_DashBoardUI);
	GETTER(TObjectPtr<UItemInventory>, WBP_ItemInventory);
	GETTER(TObjectPtr<UCountDownToStart>, CountDownToStartWidget)
#pragma endregion
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountDownToStart> CountDownToStartWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDashBoardUI> WBP_DashBoardUI;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemInventory> WBP_ItemInventory;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URealTimeRankingHUDBoard> RealTimeRankingBoard;
};
