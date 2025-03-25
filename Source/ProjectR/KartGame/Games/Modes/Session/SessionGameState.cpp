#include "SessionGameState.h"

#include "FastLogger.h"

void ASessionGameState::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerInfo.Init(TEXT(""), MaxPlayerCount);
	for (int i = 0; i < MaxPlayerCount; i++)
	{
		PlayerInfoMap.Add(i , TEXT(""));
	}
}

void ASessionGameState::JoinPlayer(const FString& PlayerId)
{
	ReadyMap.Add(PlayerId, false);

	FFastLogger::LogScreen(FColor::Red, TEXT("Join Player: %s"), *PlayerId);
	for (int i = 0; i < MaxPlayerCount; i++)
	{
		if (PlayerInfo[i] == TEXT(""))
		{
			PlayerInfo[i] = PlayerId;
			PlayerInfoMap[i] = PlayerId;
			return;
		}
	}
}

void ASessionGameState::LeavePlayer(const FString& PlayerId)
{
	const uint8 PlayerIndex = *PlayerInfoMap.FindKey(PlayerId);
	PlayerInfoMap.Remove(PlayerIndex);
	PlayerInfo[PlayerIndex] = TEXT("");
	
	ReadyMap.Remove(PlayerId);
}

bool ASessionGameState::IsPlayerReadyAll()
{
	for (const TTuple<FString, bool> Map : ReadyMap)
	{
		if (!Map.Value)
		{
			return false;
		}
	}

	return true;
}