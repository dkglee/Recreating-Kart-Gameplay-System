#include "SessionGameMode.h"

#include "CommonUtil.h"
#include "EnumUtil.h"
#include "FastLogger.h"
#include "OnlineSessionSettings.h"
#include "SessionGameState.h"
#include "SessionUtil.h"

void ASessionGameMode::StartGameToPlay()
{
	FString MatchType;
	FSessionUtil::GetCurrentSession()->SessionSettings.Get(FName(TEXT("MatchType")),MatchType);

	const FString MapDir = (MatchType == FCommonUtil::GetClassEnumKeyAsString(EMatchType::Item))
		? TEXT("/Game/Games/Race/Tracks/ItemTrack")
		: TEXT("/Game/Games/Race/Tracks/SpeedTrack");
	
	// TODO: 추후 들어갈 정보는 GameState에서 설정해서 들어가게 되어있다.
	const FString TravelURL = FString::Printf(TEXT("%s?listen"), *MapDir);
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