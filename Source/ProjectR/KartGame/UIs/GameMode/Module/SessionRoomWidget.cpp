#include "SessionRoomWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"

void USessionRoomWidget::InitializeData(const FOnlineSessionSearchResult& SearchResult)
{
	RoomId->SetText(FText::FromString(SearchResult.GetSessionIdStr()));
	RoomTitle->SetText(FText::FromString(TEXT("테스트를 위한 테스트")));
	RoomId->SetText(FText::FromString(SearchResult.GetSessionIdStr()));
	RoomId->SetText(FText::FromString(FString::FromInt(SearchResult.Session.NumOpenPublicConnections)));
}
