#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SessionPlayerController.generated.h"

class UReadySession;

UCLASS()
class PROJECTR_API ASessionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void UpdateSessionList(const TArray<FString>& PlayerList);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerInfo(const TArray<FString>& PlayerNameList);
	
protected:
	virtual void AcknowledgePossession(APawn* InPawn) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UReadySession> ReadySessionClass;
	
	UPROPERTY()
	TObjectPtr<UReadySession> ReadySession;

	UFUNCTION()
	void CheckAndStartGame();
};
