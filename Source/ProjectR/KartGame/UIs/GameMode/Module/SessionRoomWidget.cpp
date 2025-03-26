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
	
	FString RoomName;
	SearchResult.Session.SessionSettings.Get(TEXT("RoomName"), RoomName);

	
	RoomId->SetText(FText::FromString(SearchResult.GetSessionIdStr().Left(7)));
	RoomTitle->SetText(FText::FromString(FSessionUtil::DecodeData(RoomName)));

	const uint8 MaxPlayerCount = SearchResult.Session.SessionSettings.NumPublicConnections;
	const uint8 CurrentPlayerCount = SearchResult.Session.NumOpenPublicConnections;
	
	CurrentPlayer->SetText(FText::FromString(FString::FromInt(
		MaxPlayerCount - CurrentPlayerCount)));
	MaxPlayer->SetText(FText::FromString(FString::FromInt(MaxPlayerCount)));
}

void USessionRoomWidget::JoinSession()
{
	FSessionUtil::JoinSession(GetWorld(), SessionSearchResult,
		OnJoinSessionCompleteDelegate);
}

void USessionRoomWidget::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		GetOwningPlayer()->ClientTravel(Address, TRAVEL_Absolute);
	}
}
