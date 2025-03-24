#include "ItemModeListUI.h"

#include "CommonUtil.h"
#include "EnumUtil.h"
#include "OnlineSessionSettings.h"
#include "SessionUtil.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "KartGame/UIs/GameMode/Module/SessionRoomWidget.h"


void UItemModeListUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializeSearchType();
	UpdateToSearch();
}

void UItemModeListUI::InitializeSearchType()
{
	ItemModeSessionSearch = MakeShareable(new FOnlineSessionSearch());
	const FString MatchType = FCommonUtil::GetClassEnumKeyAsString(EMatchType::Item);
	ItemModeSessionSearch->QuerySettings.Set<FString>(FName("MatchType")
		, MatchType
		, EOnlineComparisonOp::Equals);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCompleteSearch);
}

void UItemModeListUI::UpdateToSearch()
{
	FSessionSearchData SessionSearchData;
	SessionSearchData.SessionSearch = ItemModeSessionSearch;
	SessionSearchData.OnFindSessionsCompleteDelegate = OnFindSessionsCompleteDelegate;
	FSessionUtil::SearchSession(SessionSearchData);
}

void UItemModeListUI::OnCompleteSearch(bool IsSuccess)
{
	if (!IsSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("방을 검색하는 것에 실패하였습니다."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("방 검색 성공: %d"), ItemModeSessionSearch->SearchResults.Num());
	SessionGridPanel->ClearChildren();

	for (int i = 0; i < ItemModeSessionSearch->SearchResults.Num(); i++)
	{
		USessionRoomWidget* NewSessionRoomWidget = CreateWidget<USessionRoomWidget>(
			this, SessionRoomWidgetClass);
		NewSessionRoomWidget->InitializeData(ItemModeSessionSearch->SearchResults[i]);
		SessionGridPanel->AddChildToUniformGrid(NewSessionRoomWidget,
			i / Horizon, i % Horizon);
	}

	// 남은 빈 칸의 수를 여기서 채워줘야 한다.
	for (int i = 0; i < MaxCount - ItemModeSessionSearch->SearchResults.Num(); i++)
	{
		USessionRoomWidget* NewSessionRoomWidget = CreateWidget<USessionRoomWidget>(
			this, SessionRoomWidgetClass);

		SessionGridPanel->AddChildToUniformGrid(NewSessionRoomWidget,
			i / Horizon, i % Horizon);
	}

	SessionList->ScrollToStart();
}
