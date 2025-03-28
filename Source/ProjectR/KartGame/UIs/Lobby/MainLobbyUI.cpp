#include "MainLobbyUI.h"

#include "EnumUtil.h"
#include "OnlineSessionSettings.h"
#include "SessionUtil.h"
#include "Components/Button.h"
#include "KartGame/Games/Component/WidgetControlComponent.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"
#include "SubUI/GameLobbySubUI.h"

void UMainLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameLobbySubUI->GetGameSearchButton()->OnClicked.AddDynamic(this, &ThisClass::OpenGameModeLobby);
	GameLobbySubUI->GetGameQuickJoinButton()->OnClicked.AddDynamic(this, &ThisClass::SearchGameAndJoinSessions);
	
	OnJoinSessionCompleteDelegate =
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSession);
}

void UMainLobbyUI::SearchGameAndJoinSessions()
{
	SessionSearchData.SessionSearch = MakeShareable(new FOnlineSessionSearch());
	const FString MatchType = FCommonUtil::GetClassEnumKeyAsString(EMatchType::Item);
	SessionSearchData.SessionSearch->QuerySettings.Set<FString>(FName("MatchType")
		, MatchType, EOnlineComparisonOp::Equals);
	SessionSearchData.OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
		this, &ThisClass::OnCompleteSearch);
	
	FSessionUtil::SearchSession(SessionSearchData);
}

void UMainLobbyUI::OnCompleteSearch(bool IsSuccess)
{
	if (!IsSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("방을 검색하는 것에 실패하였습니다."));
		return;
	}

	for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.Num(); i++)
	{
		FSessionUtil::JoinSession(GetWorld(), SessionSearchData.SessionSearch->SearchResults[i],
			OnJoinSessionCompleteDelegate);
	}
}

void UMainLobbyUI::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		GetOwningPlayer()->ClientTravel(Address, TRAVEL_Absolute);
	}
}

void UMainLobbyUI::SetDefaultWidgetInfo()
{
	IWidgetStackInterface::SetDefaultWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->GetWidgetControlComponent()->OnTriggerInputKey_C_Notified
		.AddDynamic(this, &ThisClass::OpenGameModeLobby);
	PC->GetWidgetControlComponent()->OnTriggerInputKey_F5_Notified
		.AddDynamic(this, &ThisClass::SearchGameAndJoinSessions);
}

void UMainLobbyUI::ClearWidgetInfo()
{
	IWidgetStackInterface::ClearWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}
	
	PC->GetWidgetControlComponent()->OnTriggerInputKey_C_Notified
		.RemoveDynamic(this, &ThisClass::OpenGameModeLobby);
	PC->GetWidgetControlComponent()->OnTriggerInputKey_F5_Notified
		.RemoveDynamic(this, &ThisClass::SearchGameAndJoinSessions);
}

void UMainLobbyUI::OpenGameModeLobby()
{
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->GetWidgetControlComponent()->PushWidgetStack(EUIKey::GameModeList);
}