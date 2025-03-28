#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

class UPingManagerComponent;
class AKart;
class UTrackLoadingUI;
class UMainUI;

UCLASS()
class PROJECTR_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARacePlayerController();
	
	void SetHUDToStart();

	UFUNCTION(Client, Reliable)
	void Client_SetHUDToStart();

	void CountDownToEndGame();

	UFUNCTION(Client, Reliable)
	void Client_CountDownToEndGame();

	void SpawnKartWithCheckPoint(const uint8 Index);
	
#pragma region GetterSetter
	GETTER(TObjectPtr<UMainUI>, MainHUD);
	
#pragma endregion

protected:
	virtual void BeginPlay() override;
	
private:
#pragma region UIFactory
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainUI> MainHUDClass;
#pragma endregion

	UPROPERTY()
	TObjectPtr<UPingManagerComponent> PingManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UMainUI> MainHUD;

	UFUNCTION()
	void KartSetToMove();

	UFUNCTION()
	void EndGame();

	UFUNCTION()
	void StartGameToGameState();
};