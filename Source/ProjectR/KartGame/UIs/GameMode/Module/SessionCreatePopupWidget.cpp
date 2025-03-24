#include "SessionCreatePopupWidget.h"

#include "EnumUtil.h"
#include "SessionUtil.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/SpinBox.h"

void USessionCreatePopupWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnSessionCreated);
	CreateRoomButton->OnClicked.AddDynamic(this, &ThisClass::OnClickCreateRoomButton);
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
}
