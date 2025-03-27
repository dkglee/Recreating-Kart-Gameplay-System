#include "ReadySessionPlayerWidget.h"

#include "Components/TextBlock.h"

void UReadySessionPlayerWidget::InitializeData(const FString& PlayerNickName)
{
	PlayerName->SetText(FText::FromString(PlayerNickName));
}