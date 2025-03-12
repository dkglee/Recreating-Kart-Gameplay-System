#include "RacePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SetHUDToStart, 5.0f);
}

void ARacePlayerController::SetHUDToStart()
{
	if (IsLocalController())
    {
    	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
    	MainHUD->AddToViewport();
    }
}
