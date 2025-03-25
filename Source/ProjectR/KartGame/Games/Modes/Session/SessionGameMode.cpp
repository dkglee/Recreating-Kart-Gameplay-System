#include "SessionGameMode.h"

#include "FastLogger.h"
#include "SessionGameState.h"
#include "SessionUtil.h"
        
void ASessionGameMode::StartGameToPlay()
{
	// TODO: 추후 들어갈 정보는 GameState에서 설정해서 들어가게 되어있다.
	const FString TravelURL = FString::Printf(TEXT("%s?listen"),
		TEXT("/Game/Games/Race/Tracks/F1_Racing_Track"));
	GetWorld()->ServerTravel(TravelURL);
}

void ASessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	FFastLogger::LogScreen(FColor::Red, TEXT("Join Player"));
	GetGameState<ASessionGameState>()->JoinPlayer(FSessionUtil::GetSteamIdByController(NewPlayer));
}

void ASessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	const APlayerController* PC = Cast<APlayerController>(Exiting);
	if (!PC)
	{
		return;
	}
	
	GetGameState<ASessionGameState>()->JoinPlayer(FSessionUtil::GetSteamIdByController(PC));
}