#include "RaceGameMode.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ARaceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

AActor* ARaceGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	uint8 PlayerId = 0;
	if (Player && Player->PlayerState)
	{
		PlayerId = Player->PlayerState->GetPlayerId();
	}
	
	const FString TagToFind = FString::Printf(TEXT("Player-Start-%d"),
		PlayerId + 1);
	
	TArray<AActor*> ResultList; 
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),
		FName(*TagToFind), ResultList);

	if (ResultList.Num() > 0)
	{
		return ResultList[0];
	}

	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}
