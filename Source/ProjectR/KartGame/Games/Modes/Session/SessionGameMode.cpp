#include "SessionGameMode.h"

void ASessionGameMode::StartGameToPlay()
{
	// TODO: 추후 들어갈 정보는 GameState에서 설정해서 들어가게 되어있다.
	const FString TravelURL = FString::Printf(TEXT("%s?listen"),
		TEXT("/Game/Games/Race/Tracks/F1_Racing_Track"));
	GetWorld()->ServerTravel(TravelURL);
}