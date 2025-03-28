#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SessionPlayerController.generated.h"

class UReadySession;
class UInputMappingContext;
class UWidgetControlComponent;

UCLASS()
class PROJECTR_API ASessionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASessionPlayerController();

	void UpdateSessionList(const TArray<FString>& PlayerList);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerInfo(const TArray<FString>& PlayerNameList);
	
protected:
	virtual void AcknowledgePossession(APawn* InPawn) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetControlComponent> WidgetControlComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UReadySession> ReadySessionClass;
	
	UPROPERTY()
	TObjectPtr<UReadySession> ReadySession;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> IMC_Session;

	UFUNCTION()
	void CheckAndStartGame();
};
