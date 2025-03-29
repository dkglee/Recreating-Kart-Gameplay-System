#include "RacePlayerController.h"

#include "Blueprint/UserWidget.h"

#include "Kart.h"
#include "RaceGameMode.h"
#include "RaceGameState.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "GameFramework/PlayerStart.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToEnd.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"
#include "KartGame/Games/Component/PingManagerComponent.h"
#include "Kismet/GameplayStatics.h"

ARacePlayerController::ARacePlayerController()
{
	PingManagerComponent = CreateDefaultSubobject<UPingManagerComponent>("Ping Manager Component");
	PingManagerComponent->SetNetAddressable();
	PingManagerComponent->SetIsReplicated(true);
}

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();
	PingManagerComponent->OnAllPingAccessNotified.AddDynamic(this, &ThisClass::StartGameToGameState);

	CreateMainHUD();
}

void ARacePlayerController::CreateMainHUD()
{
	if (!IsLocalController())
	{
		return;
	}

	if (MainHUD)
	{
		return;
	}
	
	MainHUD = CreateWidget<UMainUI>(this, MainHUDClass);
	MainHUD->AddToViewport();
}

void ARacePlayerController::SetHUDToStart()
{
	if (HasAuthority())
	{
		GetPawn<AKart>()->SetbCanMove(false);
	}

	if (IsLocalController())
    {
		CreateMainHUD();
		
		MainHUD->GetCountDownToStartWidget()
			->OnGameStartNotified.AddDynamic(this, &ThisClass::KartSetToMove);
		MainHUD->GetCountDownToEndWidget()->OnGameEndNotified.AddDynamic(this
			, &ThisClass::EndGame);
		MainHUD->InitializeData();
		MainHUD->StartGameUI();
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

void ARacePlayerController::SpawnKartWithCheckPoint(const uint8 Index)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(), APlayerStart::StaticClass(), Actors);

	// Player Start가 2개 이상 있는 곳은 트랙이라고 명시적으로 가정한다.
	if (Actors.Num() >= 2)
	{
		GetPawn<AKart>()->SetbCanMove(false);
	}
	
	for (AActor* Actor : Actors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);

		if (PlayerStart->PlayerStartTag == FName(FString::Printf(TEXT("Player_Start_%d"), Index)))
		{
			GetPawn()->SetActorTransform(PlayerStart->GetTransform());
			return;
		}
	}
}

void ARacePlayerController::StartGameToGameState()
{
	GetWorld()->GetAuthGameMode<ARaceGameMode>()->StartGame();
}
