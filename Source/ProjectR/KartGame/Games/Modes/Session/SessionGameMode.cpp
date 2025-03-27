#include "SessionGameMode.h"

#include "SessionGameState.h"

void ASessionGameMode::StartGameToPlay()
{
	// TODO: 추후 들어갈 정보는 GameState에서 설정해서 들어가게 되어있다.
	const FString TravelURL = FString::Printf(TEXT("%s?listen"),
		TEXT("/Game/Games/Race/Tracks/AlphaTestMap"));
	GetWorld()->ServerTravel(TravelURL);
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	GetGameState<ASessionGameState>()->JoinPlayer(NewPlayer);
}

void ASessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	if (APlayerController* PC = Cast<APlayerController>(Exiting))
	{
		GetGameState<ASessionGameState>()->LeavePlayer(PC);
	}
}