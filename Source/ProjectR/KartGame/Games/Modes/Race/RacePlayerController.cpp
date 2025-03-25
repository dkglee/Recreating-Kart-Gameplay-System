#include "RacePlayerController.h"

#include "Kart.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		this, &ThisClass::SetHUDToStart, 5.0f);

	// AKart* Kart = Cast<AKart>(GetPawn());
	// Kart->SetbCanMove(true);
}

void ARacePlayerController::SetHUDToStart()
{
	if (IsLocalController())
    {
    	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
    	MainHUD->AddToViewport();
		MainHUD->GetCountDownToStartWidget()
			->OnGameStartNotified.AddDynamic(this, &ThisClass::KartSetToMove);
    }
}

void ARacePlayerController::KartSetToMove()
{
	AKart* Kart = Cast<AKart>(GetPawn());
	Kart->SetbCanMove(true);
}

