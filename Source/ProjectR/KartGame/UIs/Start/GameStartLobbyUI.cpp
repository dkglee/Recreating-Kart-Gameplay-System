#include "GameStartLobbyUI.h"

#include "Components/WidgetSwitcher.h"
#include "SubUI/GameStartCinematicUI.h"
#include "SubUI/LicenseUI.h"

void UGameStartLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartCinematicUI->OnCinematicEndNotified.Clear();
	StartCinematicUI->OnCinematicEndNotified.AddDynamic(this, &ThisClass::GoNextSubUI);
	LicenseUI->OnLicenseEndNotified.AddDynamic(this, &ThisClass::EndSubUI);
}

void UGameStartLobbyUI::GoNextSubUI()
{
	StartSubUISwitcher->SetActiveWidgetIndex(StartSubUISwitcher->GetActiveWidgetIndex() + 1);
}

void UGameStartLobbyUI::EndSubUI()
{
	StartSubUISwitcher->SetVisibility(ESlateVisibility::Hidden);
	LobbySubUISwitcher->SetVisibility(ESlateVisibility::Collapsed);
}