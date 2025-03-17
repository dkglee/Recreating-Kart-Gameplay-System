#include "GameStartUI.h"

#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "SubUI/GameStartCinematicUI.h"
#include "SubUI/LicenseUI.h"

void UGameStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartCinematicUI->OnCinematicEndNotified.Clear();
	StartCinematicUI->OnCinematicEndNotified.AddDynamic(this, &ThisClass::GoNextSubUI);
	LicenseUI->OnLicenseEndNotified.AddDynamic(this, &ThisClass::EndSubUI);
}

void UGameStartUI::GoNextSubUI()
{
	StartSubUISwitcher->SetActiveWidgetIndex(StartSubUISwitcher->GetActiveWidgetIndex() + 1);
}

void UGameStartUI::EndSubUI()
{
	RemoveFromParent();

	// 로컬 컨트롤러 가져옴
	UGameplayStatics::GetPlayerController(GetWorld(), 0);
}