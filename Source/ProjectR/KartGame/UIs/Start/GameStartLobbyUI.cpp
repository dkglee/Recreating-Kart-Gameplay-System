#include "GameStartLobbyUI.h"

#include "Components/WidgetSwitcher.h"
#include "SubUI/GameStartCinematicUI.h"

void UGameStartLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartCinematicUI->OnAnimationEndNotified.Clear();
	StartCinematicUI->OnAnimationEndNotified.BindDynamic(this, &ThisClass::GoNextSubUI);
	BindToAnimationFinished(StartCinematicUI->GetCinematicEndNearAnimation(), StartCinematicUI->OnAnimationEndNotified);
}

void UGameStartLobbyUI::GoNextSubUI()
{
	UE_LOG(LogTemp, Display, TEXT("제 발 좀 되 라"))
	UISwitcher->SetActiveWidgetIndex(UISwitcher->GetActiveWidgetIndex() + 1);
}