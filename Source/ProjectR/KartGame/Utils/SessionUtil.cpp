#include "SessionUtil.h"

#include "CommonUtil.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

IOnlineSessionPtr FSessionUtil::OnlineSessionInterface;

void FSessionUtil::Init()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem)
	{
		return;
	}

	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
}

void FSessionUtil::CreateSession(const FSessionCreateData& SessionCreateData)
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
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		UE_LOG(LogTemp, Error, TEXT("하지만, 못난 프로그래머를 위해 세션을 제거해드립니다."));
	}
	
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
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

	OnlineSessionInterface->CreateSession(0, NAME_GameSession, *SessionSettings);
}

void FSessionUtil::SearchSession(FSessionSearchData& SessionSearchData)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
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
	
	OnlineSessionInterface->FindSessions(0,
		SessionSearchData.SessionSearch.ToSharedRef());
}

void FSessionUtil::JoinSession(const UWorld* World
	, FOnlineSessionSearchResult& Result
	, const FOnJoinSessionCompleteDelegate& OnJoinSessionCompleteDelegate)
{
	const ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();

	Result.Session.SessionSettings.bUsesPresence = true;
	Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
	
	OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
}
