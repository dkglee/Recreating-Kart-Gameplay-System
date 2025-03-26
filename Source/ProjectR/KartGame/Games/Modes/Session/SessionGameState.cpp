#include "SessionGameState.h"

#include "SessionUtil.h"
#include "SessionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASessionGameState::ASessionGameState()
{
	PlayerInfo.Init(TEXT(""), MaxPlayerCount);
	for (int i = 0; i < MaxPlayerCount; i++)
	{
		PlayerInfoMap.Add(i , TEXT(""));
	}
}

void ASessionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	 DOREPLIFETIME(ASessionGameState, PlayerInfo);
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
		ASessionPlayerController* PC = Cast<ASessionPlayerController>(PlayerState->GetPlayerController());
		PC->UpdateSessionList();
	}
}

void ASessionGameState::LeavePlayer(APlayerController* PlayerController)
{
	const FString PlayerName = FSessionUtil::GetSteamIdByController(PlayerController);
	const uint8 PlayerIndex = *PlayerInfoMap.FindKey(PlayerName);
	PlayerInfoMap.Remove(PlayerIndex);
	PlayerInfo[PlayerIndex] = TEXT("");
	
	ReadyMap.Remove(PlayerName);

	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ASessionPlayerController* PC = Cast<ASessionPlayerController>(PlayerState->GetPlayerController());
		PC->UpdateSessionList();
	}
}

void ASessionGameState::OnRep_PlayerInfo()
{
	ASessionPlayerController* PC = Cast<ASessionPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!PC)
	{
		return;
	}

	PC->UpdateSessionList();
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
