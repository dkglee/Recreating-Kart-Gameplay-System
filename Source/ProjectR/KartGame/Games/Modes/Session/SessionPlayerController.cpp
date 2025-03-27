#include "SessionPlayerController.h"

#include "Kart.h"
#include "SessionGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EnhancedInputSubsystems.h"
#include "KartGame/Games/Component/WidgetControlComponent.h"
#include "KartGame/UIs/Session/ReadySession.h"
#include "KartGame/UIs/_Common/Button/CommonButton.h"

ASessionPlayerController::ASessionPlayerController()
{
	WidgetControlComponent = CreateDefaultSubobject<UWidgetControlComponent>("Widget Control Component");
}

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
	
	if (!IsLocalPlayerController())
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(IMC_Session, 0);
	}
	
	SetShowMouseCursor(true);
	
	if (!ReadySession)
    {
    	ReadySession = CreateWidget<UReadySession>(this, ReadySessionClass);
    	ReadySession->GetGameStartButton()->GetRoot()->
    		OnClicked.AddDynamic(this, &ThisClass::CheckAndStartGame);
    	ReadySession->InitializeData();
    	ReadySession->AddToViewport();
    }
}

void ASessionPlayerController::CheckAndStartGame()
{
	GetWorld()->GetAuthGameMode<ASessionGameMode>()->StartGameToPlay();
}

void ASessionPlayerController::UpdateSessionList(const TArray<FString>& PlayerList)
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
		ReadySession->InitializeData();
		ReadySession->AddToViewport();
	}
	
	ReadySession->UpdatePlayers(PlayerList);
}

void ASessionPlayerController::Client_UpdatePlayerInfo_Implementation(
	const TArray<FString>& PlayerNameList)
{
	UpdateSessionList(PlayerNameList);
}
