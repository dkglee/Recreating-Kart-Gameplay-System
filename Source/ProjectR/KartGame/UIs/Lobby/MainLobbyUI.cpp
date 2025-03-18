#include "MainLobbyUI.h"

#include "Components/Button.h"
#include "KartGame/Games/KartGameInstance.h"
#include "SubUI/GameLobbySubUI.h"

void UMainLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameLobbySubUI->GetGameMoveButton()->OnClicked.AddDynamic(this, &ThisClass::CreateGameSessions);
}

void UMainLobbyUI::CreateGameSessions()
{
	GetGameInstance<UKartGameInstance>()->CreateNewGameSession();
}
