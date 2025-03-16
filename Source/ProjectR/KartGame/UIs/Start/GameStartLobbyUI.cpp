#include "GameStartLobbyUI.h"

#include "Components/WidgetSwitcher.h"
#include "SubUI/GameStartCinematicUI.h"

void UGameStartLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartCinematicUI->OnCinematicEndNotified.Clear();
	StartCinematicUI->OnCinematicEndNotified.AddDynamic(this, &ThisClass::GoNextSubUI);
}

void UGameStartLobbyUI::GoNextSubUI()
{
	UE_LOG(LogTemp, Display, TEXT("제 발 좀 되 라"))
	UISwitcher->SetActiveWidgetIndex(UISwitcher->GetActiveWidgetIndex() + 1);
}