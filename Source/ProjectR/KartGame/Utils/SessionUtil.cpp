#include "SessionUtil.h"

#include "CommonUtil.h"
#include "EnumUtil.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

IOnlineSessionPtr USessionUtil::OnlineSessionInterface;

FDelegateHandle USessionUtil::OnCreateSessionCompleteDelegateHandle;
FDelegateHandle USessionUtil::OnFindSessionsCompleteDelegateHandle;
FDelegateHandle USessionUtil::OnJoinSessionCompleteDelegateHandle;

void USessionUtil::Init()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem)
	{
		return;
	}

	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
}

void USessionUtil::CreateSession(const FSessionCreateData& SessionCreateData)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	const FNamedOnlineSession* ExistSession =
		OnlineSessionInterface->GetNamedSession(FName(*SessionCreateData.RoomName));
	if (ExistSession)
	{
		UE_LOG(LogTemp, Error, TEXT("이미 만들어진 세션이 존재합니다. 보통 내부 로직 이슈입니다."));
		OnlineSessionInterface->DestroySession(FName(*SessionCreateData.RoomName));
		UE_LOG(LogTemp, Error, TEXT("하지만, 못난 프로그래머를 위해 세션을 제거해드립니다."));
	}
	
	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(
		OnCreateSessionCompleteDelegateHandle);
	
	OnCreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		SessionCreateData.OnCreateSessionCompleteDelegate);

	const TSharedPtr<FOnlineSessionSettings> SessionSettings =
		MakeShareable(new FOnlineSessionSettings());
	
	SessionSettings->bIsLANMatch = false;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->bUsesPresence = true;
	
	SessionSettings->NumPublicConnections = SessionCreateData.MaxPlayer;
	SessionSettings->bShouldAdvertise = SessionCreateData.IsPublic;
	
	SessionSettings->Set(FName("MatchType"), FCommonUtil::GetClassEnumKeyAsString(
		SessionCreateData.MatchType), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	OnlineSessionInterface->CreateSession(0,
		FName(*SessionCreateData.RoomName), *SessionSettings);
}

void USessionUtil::SearchSession(FSessionSearchData& SessionSearchData)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(
		OnFindSessionsCompleteDelegateHandle);
	
	OnFindSessionsCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		SessionSearchData.OnFindSessionsCompleteDelegate);
	
	// 단순 C++ 객체를 Unreal GC로 이전시켜 관리한다.
	SessionSearchData.SessionSearch = MakeShareable(new FOnlineSessionSearch());
	// 최대 검색 수 30개로 제한
	SessionSearchData.SessionSearch->MaxSearchResults = 30;
	// Lan 검색이 아닌 온라인 검색으로 처리
	SessionSearchData.SessionSearch->bIsLanQuery = false;
	// 검색에 필요한 쿼리 세팅
	SessionSearchData.SessionSearch->QuerySettings.Set(
		SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	// SessionSearchData.SessionSearch->QuerySettings.Set(
 //        FName("MatchType"), FCommonUtil::GetClassEnumKeyAsString(
 //            EMatchType::Item), EOnlineComparisonOp::Equals);
	SessionSearchData.SessionSearch->QuerySettings.Set(
		FName(TEXT("TEAM")), FString(TEXT("Wanted")), EOnlineComparisonOp::Equals);
	
	OnlineSessionInterface->FindSessions(0,
		SessionSearchData.SessionSearch.ToSharedRef());
}

void USessionUtil::JoinSession(const UWorld* World
	, FOnlineSessionSearchResult& Result
	, const FOnJoinSessionCompleteDelegate& OnJoinSessionCompleteDelegate)
{
	const ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();

	Result.Session.SessionSettings.bUsesPresence = true;
	Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;

	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	OnJoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
}
