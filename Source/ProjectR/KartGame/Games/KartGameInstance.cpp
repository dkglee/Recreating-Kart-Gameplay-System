#include "KartGameInstance.h"

#include "FastLogger.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

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
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
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
		UE_LOG(LogTemp, Error, TEXT("이미 만들어진 세션이 존재합니다. 보통 내부 로직 이슈입니다."))
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		UE_LOG(LogTemp, Error, TEXT("하지만, 못난 프로그래머를 위해 세션을 제거해드립니다."))
		return;
	}
	
	// Shared Ptr로 처리해서 내부 함수에서 만들어진 세팅 값을 외부에서 계속 사용하더라도,
	// 더이상 사용하지 않을 때 알아서 언리얼 GC에서 관리될 수 있도록 처리해준다.
	// TODO: 튜토리얼대로 해서 우선은 이리 처리되나 좋은 방법은 아니라고 판단해 리팩토링해야한다.
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	// 랜이 아닌 온라인 멀티를 기반으로 진행하게 우선 처리함.
	SessionSettings->bIsLANMatch = false;
	// 최대 인원수
	SessionSettings->NumPublicConnections = 8;
	// 게임 플레이 중 들어올 수 있는가? (카트 특징 상 불가하게 처리)
	SessionSettings->bAllowJoinInProgress = false;
	// 친구 시스템을 통한 참가 가능 여부
	SessionSettings->bAllowJoinViaPresence = true;
	// 광고 가능여부: TODO: 비밀방인지 아닌지 여부 체크에 활용 가능
	SessionSettings->bShouldAdvertise = true;
	// 플레이어 접속한 플랫폼 표시 (ex. 스팀, xbox)
	SessionSettings->bUsesPresence = true;
	// 로비 시스템 활용 여부
	SessionSettings->bUseLobbiesIfAvailable = true;
	// 로비 시스템 사용 시 보이스챗 활용 여부
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = false;

	// 현재 매치 타입이 아이템전임을 의미함
	SessionSettings->Set(FName("MatchType"), FString("ItemMode"),
		EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	// 로컬 플레이어 고유 ID를 기반으로 세션 생성
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UKartGameInstance::OnSessionCreated(FName SessionName, bool IsCreateSuccess)
{
	if (IsCreateSuccess)
	{
		// 호스팅 전용 이동 로비
		GetWorld()->ServerTravel(FString("/Games/Race/MainRoom?listen"));
	}
}

void UKartGameInstance::SearchGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("세션이 현재 존재하지 않습니다"));
		return;
	}
	
	// 단순 C++ 객체를 Unreal GC로 이전시켜 관리한다.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	// 최대 검색 수 30개로 제한
	SessionSearch->MaxSearchResults = 30;
	// Lan 검색이 아닌 온라인 검색으로 처리
	SessionSearch->bIsLanQuery = false;
	// 검색에 필요한 쿼리 세팅
	// SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),
		SessionSearch.ToSharedRef());
}

void UKartGameInstance::OnFindSession(bool IsSuccess)
{
	FFastLogger::LogScreen(FColor::Yellow, TEXT("나 강림: %d"), SessionSearch->SearchResults.Num());
	for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString UserName = Result.Session.OwningUserName;
		
		FFastLogger::LogScreen(IsSuccess ? FColor::Green : FColor::Red,
			TEXT("하이 룸: %s, %s"), *Id, *UserName);
	}
}

void UKartGameInstance::JoinGameSession(const FOnlineSessionSearchResult& Result)
{
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
}

void UKartGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;

	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		FFastLogger::LogScreen(FColor::Magenta, TEXT("하이 브로: %s"), *Address);
	}

	GetFirstLocalPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
}
