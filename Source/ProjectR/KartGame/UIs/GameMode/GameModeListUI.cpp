#include "GameModeListUI.h"

#include "Components/Button.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"
#include "Module/SessionCreatePopupWidget.h"

void UGameModeListUI::NativeConstruct()
{
	Super::NativeConstruct();
	SessionCreatePopup->SetVisibility(ESlateVisibility::Hidden);
	SessionCreateButton->OnClicked.AddDynamic(this, &ThisClass::OpenSessionCreate);
}

void UGameModeListUI::OpenSessionCreate()
{
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	// 숨겨져 있는 상태인 경우에 대한 처리
	if (SessionCreatePopup->GetVisibility() == ESlateVisibility::Visible)
	{
		return;
	}
	
	SessionCreatePopup->SetVisibility(ESlateVisibility::Visible);
	if (PC)
	{
		PC->OnClickInputKey_Esc_Notified.AddDynamic(this, &ThisClass::RemoveSessionCreate);
	}
}

void UGameModeListUI::RemoveSessionCreate()
{
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	SessionCreatePopup->SetVisibility(ESlateVisibility::Hidden);
	if (PC)
	{
		PC->OnClickInputKey_Esc_Notified.RemoveDynamic(this, &ThisClass::RemoveSessionCreate);
	}
}

void UGameModeListUI::SetDefaultWidgetInfo()
{
	IWidgetStackInterface::SetDefaultWidgetInfo();
	
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (!PC)
	{
		return;
	}

	PC->OnClickInputKey_C_Notified.AddDynamic(this, &ThisClass::OpenSessionCreate);
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
	
	PC->OnClickInputKey_C_Notified.RemoveDynamic(this, &ThisClass::OpenSessionCreate);
}
