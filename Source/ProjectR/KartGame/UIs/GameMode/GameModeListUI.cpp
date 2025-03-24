#include "GameModeListUI.h"

#include "Components/Button.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"
#include "Module/SessionCreatePopupWidget.h"

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

	PC->OnClickInputKey_C_Notified.AddDynamic(SessionCreatePopup,
		&USessionCreatePopupWidget::OpenSessionCreate);
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
	
	PC->OnClickInputKey_C_Notified.RemoveDynamic(SessionCreatePopup,
		&USessionCreatePopupWidget::OpenSessionCreate);
}
