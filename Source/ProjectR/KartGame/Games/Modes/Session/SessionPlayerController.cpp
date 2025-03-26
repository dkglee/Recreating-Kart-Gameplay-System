#include "SessionPlayerController.h"

#include "Kart.h"
#include "SessionGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "KartGame/UIs/Session/ReadySession.h"
#include "KartGame/UIs/_Common/Button/CommonButton.h"

void ASessionPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);

	// 카트가 아니면 안되는 상황이기 때문에 예외처리 안함
	AKart* KartBody = Cast<AKart>(InPawn);
	KartBody->SetbCanMove(false);
}

void ASessionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		SetShowMouseCursor(true);
	}
}

void ASessionPlayerController::CheckAndStartGame()
{
	GetWorld()->GetAuthGameMode<ASessionGameMode>()->StartGameToPlay();
}

void ASessionPlayerController::UpdateSessionList()
{
	if (!IsLocalPlayerController())
	{
		return;
	}
	
	if (!ReadySession)
	{
		ReadySession = CreateWidget<UReadySession>(this, ReadySessionClass);
		ReadySession->GetGameStartButton()->GetRoot()->
			OnClicked.AddDynamic(this, &ThisClass::CheckAndStartGame);
		ReadySession->AddToViewport();
	}
	
	ReadySession->UpdatePlayers();
}
