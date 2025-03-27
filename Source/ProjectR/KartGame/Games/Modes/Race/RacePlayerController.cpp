#include "RacePlayerController.h"

#include "Kart.h"
#include "RaceGameState.h"
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
	// 리슨서버 기준으로 게임 시작은 서버가 정의한다.
	if (HasAuthority())
	{
		GetWorld()->GetGameState<ARaceGameState>()
			->SetRaceStartTime(FDateTime::Now());
		GetWorld()->GetGameState<ARaceGameState>()
			->SetRaceStatus(ERaceStatus::Playing);
	}
	
	AKart* Kart = Cast<AKart>(GetPawn());
	Kart->SetbCanMove(true);
}