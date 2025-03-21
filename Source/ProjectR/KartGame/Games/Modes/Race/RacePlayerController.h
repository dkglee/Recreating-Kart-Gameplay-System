#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

class UMainUI;
/**
 * 
 */
UCLASS()
class PROJECTR_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
#pragma region GetterSetter
	GETTER(TObjectPtr<UMainUI>, MainHUD);
#pragma endregion
	
protected:
	virtual void BeginPlay() override;
	
private:
	FTimerHandle TimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainUI> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainHUD;

	void SetHUDToStart();

	UFUNCTION()
	void KartSetToMove();
};
