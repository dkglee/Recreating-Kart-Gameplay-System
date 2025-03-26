#include "SessionPlayerState.h"

#include "SessionGameState.h"
#include "SessionUtil.h"
#include "Net/UnrealNetwork.h"

void ASessionPlayerState::BeginPlay()
{
	Super::BeginPlay();

	PlayerDisplayName = FSessionUtil::GetSteamIdByController(GetPlayerController());
	GetWorld()->GetGameState<ASessionGameState>()->JoinPlayer(GetPlayerController());
}

void ASessionPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASessionPlayerState, PlayerDisplayName);
}
