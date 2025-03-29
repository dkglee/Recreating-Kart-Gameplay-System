#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaceGameMode.generated.h"

UCLASS()
class PROJECTR_API ARaceGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void StartGame();
	
protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle GameStartTimerHandle;

	UFUNCTION()
	void SetReadyGame();
};