#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "KartGame/UIs/Lobby/MainLobbyUI.h"
#include "KartGame/UIs/GameMode/GameModeListUI.h"
#include "KartGame/Games/Component/WidgetControlComponent.h"
#include "EnumUtil.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	WidgetControlComponent =
		CreateDefaultSubobject<UWidgetControlComponent>("Widget Control Component");
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(IMC_Lobby, 0);
	}
	
	SetShowMouseCursor(true);
	
	UMainLobbyUI* MainLobbyUI = CreateWidget<UMainLobbyUI>(this, MainLobbyUIClass);
	WidgetControlComponent->Initialize(MainLobbyUI);
	WidgetControlComponent->AddWidgetData(EUIKey::GameModeList,
		CreateWidget<UGameModeListUI>(this, GameModeListUIClass));
}