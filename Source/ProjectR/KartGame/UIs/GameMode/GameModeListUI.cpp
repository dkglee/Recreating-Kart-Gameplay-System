#include "GameModeListUI.h"

#include "Components/Button.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"
#include "Module/SessionCreatePopupWidget.h"

void UGameModeListUI::NativeConstruct()
{
	Super::NativeConstruct();
	SessionCreatePopup->SetVisibility(ESlateVisibility::Hidden);
	SessionCreateButton->OnClicked.AddDynamic(this, &ThisClass::ClickToSessionCreate);
}

void UGameModeListUI::ClickToSessionCreate()
{
	SessionCreatePopup->SetVisibility(
		SessionCreatePopup->GetVisibility() == ESlateVisibility::Hidden ?
		ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGameModeListUI::SetDefaultWidgetInfo()
{
	IWidgetStackInterface::SetDefaultWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->OnClickInputKey_C_Notified.AddDynamic(this, &ThisClass::ClickToSessionCreate);
}

void UGameModeListUI::ClearWidgetInfo()
{
	IWidgetStackInterface::ClearWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}
	
	PC->OnClickInputKey_C_Notified.RemoveDynamic(this, &ThisClass::ClickToSessionCreate);
}
