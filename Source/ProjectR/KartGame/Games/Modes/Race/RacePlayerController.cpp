#include "RacePlayerController.h"

#include "FastLogger.h"
#include "Blueprint/UserWidget.h"

#include "Kart.h"
#include "KartFrictionComponent.h"
#include "RaceGameMode.h"
#include "RaceGameState.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "GameFramework/PlayerStart.h"
#include "KartGame/Games/Component/NetworkClockComponent.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToEnd.h"
#include "KartGame/UIs/HUD/CountDown/CountDownToStart.h"
#include "KartGame/Games/Component/PingManagerComponent.h"
#include "Kismet/GameplayStatics.h"

ARacePlayerController::ARacePlayerController()
{
	PingManagerComponent = CreateDefaultSubobject<UPingManagerComponent>("Ping Manager Component");
	PingManagerComponent->SetNetAddressable();
	PingManagerComponent->SetIsReplicated(true);

#pragma region NetworkClock
	NetworkClockComponent = CreateDefaultSubobject<UNetworkClockComponent>("Network Clock Component");
	NetworkClockComponent->SetNetAddressable();
	NetworkClockComponent->SetIsReplicated(true);
#pragma endregion
}

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();
	PingManagerComponent->OnAllPingAccessNotified.AddDynamic(this, &ThisClass::StartGameToGameState);

	CreateMainHUD();

	// OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPlayer);
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
	Kart->GetFrictionComponent()->OnInstantBoost.Broadcast();
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
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(FString::Printf(TEXT("Player_Start_%d"), Index)), Actors);
	
	for (AActor* Actor : Actors)
	{
		SpawnKartToTransform(Actor->GetTransform());
		return;
	}
}

void ARacePlayerController::SpawnKartToTransform(const FTransform& Transform)
{
	if (GetPawn())
	{
		return;
	}

	FTransform NewTransform;
	NewTransform.SetLocation(FVector(1000, 1000, 1000));
	AKart* Kart = GetWorld()->SpawnActor<AKart>(KartBodyClass, NewTransform);
	Possess(Kart);
	Kart->SetbCanMove(false);
	
	if (IsLocalController())
	{
		Kart->SetActorTransform(Transform);
	} else
	{
		Kart->Client_SetActorTransform(Transform);
	}
}

void ARacePlayerController::StartGameToGameState()
{
	GetWorld()->GetAuthGameMode<ARaceGameMode>()->StartGame();
}

#pragma region NetworkClock

// PlayerController가 서버로부터 생성되었을 때 호출되는 함수
// 해당 함수에서 서버 시간을 요청한다.
void ARacePlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (NetworkClockComponent)
	{
		NetworkClockComponent->RequestServerTime(this);
	}
}

void ARacePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	//
	AKart* Kart = Cast<AKart>(GetPawn());
	if (Kart)
	{
		Kart->OnPossessedPawnChanged(nullptr, nullptr);
	}
	
	if (HasAuthority())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Server Rep Pawn"));
	}
	else
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Client Rep Pawn"));
	}
}

void ARacePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AKart* Kart = Cast<AKart>(GetPawn());
	if (Kart)
	{
		Kart->OnPossessedPawnChanged(nullptr, nullptr);
	}
	
	if (HasAuthority())
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Server Rep Pawn"));
	}
	else
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Client Rep Pawn"));
	}
}

void ARacePlayerController::OnPossessedPlayer(APawn* InOldPawn, APawn* InNewPawn)
{
	// AKart* Kart = Cast<AKart>(InNewPawn);
	// if (Kart)
	// {
	// // 	Kart->OnPossessedPawnChanged(nullptr, nullptr);
	// 	if (HasAuthority())
	// 	{
	// 		FFastLogger::LogScreen(FColor::Red, TEXT("Server Rep Pawn"));
	// 	}
	// 	else
	// 	{
	// 		FFastLogger::LogScreen(FColor::Red, TEXT("Client Rep Pawn"));
	// 	}
	// }

}

float ARacePlayerController::GetServerTime() const
{
	if (NetworkClockComponent)
	{
		return NetworkClockComponent->GetServerTime();
	}
	return GetWorld()->GetTimeSeconds();
}

#pragma endregion