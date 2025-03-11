#include "RacePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
		MainHUD->AddToViewport();
	}
}
