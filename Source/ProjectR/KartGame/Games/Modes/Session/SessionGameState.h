#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/GameStateBase.h"
#include "SessionGameState.generated.h"

UCLASS()
class PROJECTR_API ASessionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASessionGameState();
	
	void JoinPlayer(const FString& PlayerName);
	void LeavePlayer(const FString& PlayerName);

	bool IsPlayerReadyAll();

	GETTER(TArray<FString>, PlayerInfo)

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 MaxPlayerCount = 8;
	
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_PlayerInfo)
	TArray<FString> PlayerInfo;

	UFUNCTION()
	void OnRep_PlayerInfo();

	// 서버에서만 처리하면 되기 때문에 서버에서만 가지고 있기
	UPROPERTY()
	TMap<uint8, FString> PlayerInfoMap;

	// TODO: PlayerState로 이전
	UPROPERTY()
	TMap<FString, bool> ReadyMap;
};
