#include "SessionGameState.h"

#include "SessionUtil.h"
#include "SessionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ASessionGameState::ASessionGameState()
{
	PlayerInfo.Init(TEXT(""), MaxPlayerCount);
	for (int i = 0; i < MaxPlayerCount; i++)
	{
		PlayerInfoMap.Add(i , TEXT(""));
	}
}

void ASessionGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ASessionGameState::JoinPlayer(APlayerController* PlayerController)
{
	const FString PlayerName = FSessionUtil::GetSteamIdByController(PlayerController);
	ReadyMap.Add(PlayerName, false);

	for (int i = 0; i < MaxPlayerCount; i++)
	{
		if (PlayerInfo[i] == TEXT(""))
		{
			PlayerInfo[i] = PlayerName;
			PlayerInfoMap[i] = PlayerName;
			break;
		}
	}

	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		Client_UpdatePlayerInfo(PlayerState->GetPlayerController(), PlayerInfo);
	}
}

void ASessionGameState::LeavePlayer(APlayerController* PlayerController)
{
	const FString PlayerName = FSessionUtil::GetSteamIdByController(PlayerController);
	const uint8 PlayerIndex = *PlayerInfoMap.FindKey(PlayerName);
	PlayerInfoMap.Remove(PlayerIndex);
	PlayerInfo[PlayerIndex] = TEXT("");
	
	ReadyMap.Remove(PlayerName);
}

void ASessionGameState::Client_UpdatePlayerInfo_Implementation(APlayerController* TargetController, const TArray<FString>& PlayerNameList)
{
	ASessionPlayerController* PC = Cast<ASessionPlayerController>(TargetController);

	if (!PC)
	{
		return;
	}

	PC->UpdateSessionList(PlayerNameList);
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
