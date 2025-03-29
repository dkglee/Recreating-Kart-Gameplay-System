#include "MainUI.h"

#include "CountDown/CountDownToStart.h"
#include "DashBoard/DashBoardUI.h"
#include "ItemInventory/ItemInventory.h"
#include "Ranking/RankingResultUI.h"
#include "Ranking/RealTimeRankingHUDBoard.h"
#include "TrackInfo/TrackInfo.h"

void UMainUI::InitializeData()
{
	RealTimeRankingBoard->InitializeData();
	TrackInfoUI->InitializeData();
}

void UMainUI::ShowResult()
{
	WBP_DashBoardUI->SetVisibility(ESlateVisibility::Hidden);
	WBP_ItemInventory->SetVisibility(ESlateVisibility::Hidden);
	RealTimeRankingBoard->SetVisibility(ESlateVisibility::Hidden);

	RankingResultUI->SetVisibility(ESlateVisibility::Visible);
	RankingResultUI->InitializeRanking();
}

void UMainUI::StartGameUI()
{
	CountDownToStartWidget->StartCountDown();
}
