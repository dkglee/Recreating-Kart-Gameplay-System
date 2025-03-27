#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

class UTrackLoadingUI;
class UMainUI;

UCLASS()
class PROJECTR_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
#pragma region GetterSetter
	GETTER(TObjectPtr<UMainUI>, MainHUD);
#pragma endregion
	void SetHUDToStart();

	UFUNCTION(Client, Reliable)
	void Client_SetHUDToStart();
	
private:
#pragma region UIFactory
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainUI> MainHUDClass;
#pragma endregion
	
	UPROPERTY()
	TObjectPtr<UMainUI> MainHUD;

	UFUNCTION()
	void KartSetToMove();
};
