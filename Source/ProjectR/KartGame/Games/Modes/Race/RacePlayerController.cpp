#include "RacePlayerController.h"

#include "FastLogger.h"
#include "Kart.h"
#include "RaceGameState.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToEnd.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"

void ARacePlayerController::SetHUDToStart()
{     
	if (IsLocalController())
    {
    	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
		MainHUD->GetCountDownToStartWidget()
			->OnGameStartNotified.AddDynamic(this, &ThisClass::KartSetToMove);
		MainHUD->GetCountDownToEndWidget()->OnGameEndNotified.AddDynamic(this
			, &ThisClass::EndGame);
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

void ARacePlayerController::CountDownToEndGame()
{
	GetMainHUD()->GetCountDownToEndWidget()->PlayCountDown();	
}

void ARacePlayerController::Client_CountDownToEndGame_Implementation()
{
	CountDownToEndGame();
}

void ARacePlayerController::EndGame()
{
	// 종료 UI 노출 처리 (해당 부분은 서버와 동기화가 굳이굳이 필요하진 않다)
	// 어차피 리타이어 관련 여부는 서버에서 체크하기 때문이다.
	MainHUD->ShowResult();
}