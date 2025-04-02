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

	UPROPERTY(EditDefaultsOnly, Category = "Options|Pawn", meta = (AllowPrivateAccess = true))
	TSubclassOf<AKart> KartBodyClass;
	
	UPROPERTY()
	TObjectPtr<UPingManagerComponent> PingManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UMainUI> MainHUD;

	void CreateMainHUD();
	
	UFUNCTION()
	void KartSetToMove();

	UFUNCTION()
	void EndGame();

	UFUNCTION()
	void StartGameToGameState();

	void SpawnKartToTransform(const FTransform& Transform);

#pragma region NetworkClock

public:
	float GetServerTime() const;
	
protected:
	// 네트워크 시계를 만들기 위해서 필요한 함수
	// 아래 함수에서 서버로부터 시간을 받아오도록 요청한다.
	virtual void ReceivedPlayer() override;

private:
	UPROPERTY()
	class UNetworkClockComponent* NetworkClockComponent = nullptr;
#pragma endregion
#pragma region Possess
	virtual void OnRep_Pawn() override;
	virtual void OnPossess(APawn* InPawn) override;
	UFUNCTION()
	void OnPossessedPlayer(APawn* OldPawn, APawn* NewPawn);
#pragma endregion
};