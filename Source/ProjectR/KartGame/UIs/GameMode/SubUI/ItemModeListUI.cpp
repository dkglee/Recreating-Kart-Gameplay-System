#include "ItemModeListUI.h"

#include "FastLogger.h"
#include "OnlineSessionSettings.h"
#include "SessionUtil.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "KartGame/UIs/GameMode/Module/SessionRoomWidget.h"

void UItemModeListUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InitializeSearchType();
}

void UItemModeListUI::InitializeSearchType()
{
	SessionSearchData.SessionSearch = MakeShareable(new FOnlineSessionSearch());
	// const FString MatchType = FCommonUtil::GetClassEnumKeyAsString(EMatchType::Item);
	// SessionSearchData.SessionSearch->QuerySettings.Set<FString>(FName("MatchType")
	// 	, MatchType, EOnlineComparisonOp::Equals);
	SessionSearchData.OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCompleteSearch);
}

void UItemModeListUI::UpdateToSearch()
{
	SessionGridPanel->ClearChildren();
	FSessionUtil::SearchSession(SessionSearchData);
}

void UItemModeListUI::OnCompleteSearch(bool IsSuccess)
{
	if (!IsSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("방을 검색하는 것에 실패하였습니다."));
		return;
	}
	FFastLogger::LogScreen(FColor::Red, TEXT("방 검색 성공: %d"), SessionSearchData.SessionSearch->SearchResults.Num());

	for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.Num(); i++)
	{
		USessionRoomWidget* NewSessionRoomWidget = CreateWidget<USessionRoomWidget>(
			this, SessionRoomWidgetClass);
		NewSessionRoomWidget->InitializeData(SessionSearchData.SessionSearch->SearchResults[i]);
		SessionGridPanel->AddChildToUniformGrid(NewSessionRoomWidget,
			i / Horizon, i % Horizon);
	}

	// 남은 빈 칸의 수를 여기서 채워줘야 한다.
	for (int i = SessionSearchData.SessionSearch->SearchResults.Num(); i < MaxCount; i++)
	{
		USessionRoomWidget* NewSessionRoomWidget = CreateWidget<USessionRoomWidget>(
			this, SessionRoomWidgetClass);

		SessionGridPanel->AddChildToUniformGrid(NewSessionRoomWidget,
			i / Horizon, i % Horizon);
	}

	SessionList->ScrollToStart();
}
