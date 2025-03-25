#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SessionGameState.generated.h"

UCLASS()
class PROJECTR_API ASessionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void JoinPlayer(const FString& PlayerId);
	void LeavePlayer(const FString& PlayerId);

	bool IsPlayerReadyAll();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxPlayerCount = 8;
	
	UPROPERTY()
	TArray<FString> PlayerInfo;
	
	UPROPERTY()
	TMap<uint8, FString> PlayerInfoMap;
	
	UPROPERTY()
	TMap<FString, bool> ReadyMap;
};
