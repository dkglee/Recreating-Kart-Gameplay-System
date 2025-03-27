#include "RacePlayerController.h"

#include "Kart.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"

void ARacePlayerController::SetHUDToStart()
{
	if (IsLocalController())
    {
    	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
		MainHUD->GetCountDownToStartWidget()
			->OnGameStartNotified.AddDynamic(this, &ThisClass::KartSetToMove);
    	MainHUD->AddToViewport();
		MainHUD->InitializeData();
    }
}

void ARacePlayerController::Client_SetHUDToStart_Implementation()
{
	SetHUDToStart();
}

void ARacePlayerController::KartSetToMove()
{
	AKart* Kart = Cast<AKart>(GetPawn());
	Kart->SetbCanMove(true);
}
