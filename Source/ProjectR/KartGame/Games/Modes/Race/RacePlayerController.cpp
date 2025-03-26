#include "RacePlayerController.h"

#include "Kart.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"
#include "KartGame/UIs/Session/TrackLoadingUI.h"

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		TrackLoadingUI = CreateWidget<UTrackLoadingUI>(this, TrackLoadingUIClass);
		TrackLoadingUI->AddToViewport();
		
		AKart* Kart = Cast<AKart>(GetPawn());
		Kart->SetbCanMove(true);
	}
}

void ARacePlayerController::SetHUDToStart()
{
	if (IsLocalController())
    {
		TrackLoadingUI->RemoveFromParent();
		
    	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
    	MainHUD->AddToViewport();
		MainHUD->GetCountDownToStartWidget()
			->OnGameStartNotified.AddDynamic(this, &ThisClass::KartSetToMove);
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
