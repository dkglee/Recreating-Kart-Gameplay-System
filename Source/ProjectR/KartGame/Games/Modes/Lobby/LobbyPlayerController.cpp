#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/Lobby/MainLobbyUI.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainLobbyUI = CreateWidget<UMainLobbyUI>(this, MainLobbyUIClass);
	MainLobbyUI->AddToViewport(0);
}
