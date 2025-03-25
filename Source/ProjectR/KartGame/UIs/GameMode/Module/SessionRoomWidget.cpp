#include "SessionRoomWidget.h"

#include "OnlineSessionSettings.h"
#include "SessionUtil.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USessionRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SessionRoomButton->OnClicked.AddDynamic(this, &ThisClass::JoinSession);

	OnJoinSessionCompleteDelegate =
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSession);
}

void USessionRoomWidget::InitializeData(const FOnlineSessionSearchResult& SearchResult)
{
	SessionSearchResult = SearchResult;
	
	RoomId->SetText(FText::FromString(SearchResult.GetSessionIdStr()));
	RoomTitle->SetText(FText::FromString(TEXT("테스트를 위한 테스트")));
	RoomId->SetText(FText::FromString(SearchResult.GetSessionIdStr()));
	RoomId->SetText(FText::FromString(FString::FromInt(SearchResult.Session.NumOpenPublicConnections)));
}

void USessionRoomWidget::JoinSession()
{
	FSessionUtil::JoinSession(GetWorld(), SessionSearchResult,
		OnJoinSessionCompleteDelegate);	
}

void USessionRoomWidget::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
}
