#include "GameModeListUI.h"

#include "Components/Button.h"
#include "KartGame/Games/Component/WidgetControlComponent.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"
#include "Module/SessionCreatePopupWidget.h"
#include "SubUI/ItemModeListUI.h"

void UGameModeListUI::NativeConstruct()
{
	Super::NativeConstruct();
	SessionCreatePopup->SetVisibility(ESlateVisibility::Hidden);
	SessionCreateButton->OnClicked.AddDynamic(SessionCreatePopup,
		&USessionCreatePopupWidget::OpenSessionCreate);
}

void UGameModeListUI::SetDefaultWidgetInfo()
{
	IWidgetStackInterface::SetDefaultWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->GetWidgetControlComponent()->OnTriggerInputKey_C_Notified
		.AddDynamic(SessionCreatePopup,
			&USessionCreatePopupWidget::OpenSessionCreate);
	
	ItemModeListUI->UpdateToSearch();
}

void UGameModeListUI::ClearWidgetInfo()
{
	IWidgetStackInterface::ClearWidgetInfo();
	SessionCreatePopup->SetVisibility(ESlateVisibility::Hidden);
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}
	
	PC->GetWidgetControlComponent()->OnTriggerInputKey_C_Notified
		.RemoveDynamic(SessionCreatePopup,
			&USessionCreatePopupWidget::OpenSessionCreate);
}