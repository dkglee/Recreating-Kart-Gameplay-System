#pragma once

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"

class FOnlineSessionSearch;
enum class EMatchType : uint8;

struct FSessionCreateData
{
	uint8 MaxPlayer;
	bool IsPublic = true;
	EMatchType MatchType;
	FString RoomName;
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};

struct FSessionSearchData
{
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
};

class FSessionUtil
{
public:
	static void Init();
	static void CreateSession(const FSessionCreateData& SessionCreateData);
	static void SearchSession(FSessionSearchData& SessionSearchData);
	static void JoinSession(const UWorld* World, FOnlineSessionSearchResult& Result,
		const FOnJoinSessionCompleteDelegate& OnJoinSessionCompleteDelegate);

private:
	static IOnlineSessionPtr OnlineSessionInterface;
};
