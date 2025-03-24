#include "SessionCreatePopupWidget.h"

#include "EnumUtil.h"
#include "SessionUtil.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/SpinBox.h"
#include "KartGame/Games/Modes/Lobby/LobbyPlayerController.h"

void USessionCreatePopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnSessionCreated);
	CreateRoomButton->OnClicked.AddDynamic(this, &ThisClass::OnClickCreateRoomButton);

	PC = Cast<ALobbyPlayerController>(GetOwningPlayer());
}

void USessionCreatePopupWidget::OpenSessionCreate()
{
	// 숨겨져 있는 상태인 경우에 대한 처리
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	if (PC)
	{
		PC->OnClickInputKey_Esc_Notified.AddDynamic(this, &ThisClass::RemoveSessionCreate);
	}
}

void USessionCreatePopupWidget::RemoveSessionCreate()
{
	SetVisibility(ESlateVisibility::Hidden);
	if (PC)
	{
		PC->OnClickInputKey_Esc_Notified.RemoveDynamic(this, &ThisClass::RemoveSessionCreate);
	}
}

void USessionCreatePopupWidget::OnClickCreateRoomButton()
{
	FSessionCreateData CreateData;
	CreateData.IsPublic = GamePublicCheckBox->GetCheckedState() == ECheckBoxState::Checked;
	CreateData.MatchType = EMatchType::Item;
	CreateData.MaxPlayer = static_cast<uint8>(RoomPlayerCounter->GetDelta());
	CreateData.RoomName = RoomTitle->GetText().ToString();
	CreateData.OnCreateSessionCompleteDelegate = OnCreateSessionCompleteDelegate;
	
	FSessionUtil::CreateSession(CreateData);
}

void USessionCreatePopupWidget::OnSessionCreated(FName SessionName, bool IsCreateSuccess)
{
	UE_LOG(LogTemp, Display, TEXT("하이 : %s"), *SessionName.ToString());
	RemoveSessionCreate();
}