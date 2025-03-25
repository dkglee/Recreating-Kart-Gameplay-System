#include "GameModeListUI.h"

#include "FastLogger.h"
#include "Components/Button.h"
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
	FFastLogger::LogScreen(FColor::Red, TEXT("도망가라 너"));
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->OnClickInputKey_C_Notified.AddDynamic(SessionCreatePopup,
		&USessionCreatePopupWidget::OpenSessionCreate);
	FFastLogger::LogScreen(FColor::Green, TEXT("나는 통과하다"));
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
	
	PC->OnClickInputKey_C_Notified.RemoveDynamic(SessionCreatePopup,
		&USessionCreatePopupWidget::OpenSessionCreate);
}
