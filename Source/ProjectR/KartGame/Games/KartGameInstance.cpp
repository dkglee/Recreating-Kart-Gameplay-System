#include "KartGameInstance.h"

#include "FastLogger.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

UKartGameInstance::UKartGameInstance()
: OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionCreated)),
OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSession)),
OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSession))
{
}

void UKartGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem)
	{
		return;
	}

	OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
}

void UKartGameInstance::CreateNewGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	// 기존에 만들어진 세션 여부 확인
	FNamedOnlineSession* ExistSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistSession)
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("하지만, 못난 프로그래머를 위해 세션을 제거해드립니다."));
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		FFastLogger::LogScreen(FColor::Red, TEXT("이미 만들어진 세션이 존재합니다. 보통 내부 로직 이슈입니다."));
	}
	
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	
	// Shared Ptr로 처리해서 내부 함수에서 만들어진 세팅 값을 외부에서 계속 사용하더라도,
	// 더이상 사용하지 않을 때 알아서 언리얼 GC에서 관리될 수 있도록 처리해준다.
	// TODO: 튜토리얼대로 해서 우선은 이리 처리되나 좋은 방법은 아니라고 판단해 리팩토링해야한다.
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = true;
	SessionSettings->NumPublicConnections = 8;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->bUsesPresence = true;

	// 현재 매치 타입이 아이템전임을 의미함
	// SessionSettings->Set(FName("MatchType"), FString("ItemMode"),
	// 	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	// 로컬 플레이어 고유 ID를 기반으로 세션 생성
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UKartGameInstance::OnSessionCreated(FName SessionName, bool IsCreateSuccess)
{
	if (IsCreateSuccess)
	{
		// 호스팅 전용 이동 로비
		GetWorld()->ServerTravel(FString("/Game/Games/Race/MainRoom?listen"));
	}
}

void UKartGameInstance::SearchGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	
	// 단순 C++ 객체를 Unreal GC로 이전시켜 관리한다.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	// 최대 검색 수 30개로 제한
	SessionSearch->MaxSearchResults = 30;
	// Lan 검색이 아닌 온라인 검색으로 처리
	SessionSearch->bIsLanQuery = true;
	// 검색에 필요한 쿼리 세팅

	
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(0,
		SessionSearch.ToSharedRef());
}

void UKartGameInstance::OnFindSession(bool IsSuccess)
{
	FFastLogger::LogScreen(FColor::Yellow, TEXT("찾은 방 갯수: %d"), SessionSearch->SearchResults.Num());
	FFastLogger::LogScreen(FColor::Yellow, TEXT("성공 여부: %d"), IsSuccess);
	for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString UserName = Result.Session.OwningUserName;

		JoinGameSession(Result);
		
		FFastLogger::LogScreen(IsSuccess ? FColor::Green : FColor::Red,
			TEXT("하이 룸: %s, %s"), *Id, *UserName);
	}
}

void UKartGameInstance::JoinGameSession(const FOnlineSessionSearchResult& Result)
{
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	
	OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
}

void UKartGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;

	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		FFastLogger::LogScreen(FColor::Magenta, TEXT("하이 브로: %s"), *Address);

		GetFirstLocalPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
	}
}