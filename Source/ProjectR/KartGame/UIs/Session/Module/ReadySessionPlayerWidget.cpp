#include "ReadySessionPlayerWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UReadySessionPlayerWidget::InitializeData(const APlayerState* PS)
{
	PlayerName->SetText(FText::FromString(PS->GetName()));
}