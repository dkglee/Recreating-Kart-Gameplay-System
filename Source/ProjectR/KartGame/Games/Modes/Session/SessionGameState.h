#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "SessionGameState.generated.h"

UCLASS()
class PROJECTR_API ASessionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASessionGameState();
	
	void JoinPlayer(APlayerController* PlayerController);
	void LeavePlayer(APlayerController* PlayerController);

	bool IsPlayerReadyAll();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxPlayerCount = 8;
	
	UPROPERTY()
	TArray<FString> PlayerInfo;

	// 서버에서만 처리하면 되기 때문에 서버에서만 가지고 있기
	UPROPERTY()
	TMap<uint8, FString> PlayerInfoMap;

	// TODO: PlayerState로 이전
	UPROPERTY()
	TMap<FString, bool> ReadyMap;

	void UpdateSessionInfo();
};