#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class UTrackInfo;
class URankingResultUI;
class UCountDownToEnd;
class UDashBoardUI;
class UItemInventory;
class URealTimeRankingHUDBoard;
class UCountDownToStart;
class UNotificationTextUI;

UCLASS()
class PROJECTR_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData();
	void ShowResult();
	
#pragma region GETTERSETTER
	GETTER(TObjectPtr<UDashBoardUI>, WBP_DashBoardUI);
	GETTER(TObjectPtr<UItemInventory>, WBP_ItemInventory);
	GETTER(TObjectPtr<UCountDownToStart>, CountDownToStartWidget);
	GETTER(TObjectPtr<UCountDownToEnd>, CountDownToEndWidget);
	GETTER(TObjectPtr<UNotificationTextUI>, WBP_NotificationTextUI);
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCountDownToEnd> CountDownToEndWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URankingResultUI> RankingResultUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNotificationTextUI> WBP_NotificationTextUI;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTrackInfo> TrackInfoUI;
};
