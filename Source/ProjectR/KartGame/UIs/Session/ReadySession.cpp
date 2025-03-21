#include "ReadySession.h"

#include "Components/UniformGridPanel.h"
#include "GameFramework/GameStateBase.h"
#include "Module/ReadySessionPlayerWidget.h"

void UReadySession::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	InitializeWidget();
}

void UReadySession::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UReadySession::InitializeWidget()
{
	if (!IsValid(PlayerInfoClass))
	{
		return;
	}

	PlayerInfoGrid->ClearChildren();

	for (int H = 0; H < PlayerInfoGridHeight; H++)
	{
		for (int W = 0; W < PlayerInfoGridWidth; W++)
		{
			UReadySessionPlayerWidget* PlayerWidget = CreateWidget<UReadySessionPlayerWidget>(this, PlayerInfoClass);
			PlayerInfoGrid->AddChildToUniformGrid(PlayerWidget, H, W);
		}
	}
}

void UReadySession::UpdatePlayers()
{
	TArray<TObjectPtr<APlayerState>> PSArray = GetWorld()->GetGameState()->PlayerArray;
	
	if (PSArray.Num() == 0)
	{
		return;
	}
	
	for (int i = 0; i < PSArray.Num(); i++)
	{
		UReadySessionPlayerWidget* ChildWidget = Cast<UReadySessionPlayerWidget>(
			PlayerInfoGrid->GetChildAt(i));
		if (!ChildWidget)
		{
			return;
		}
		ChildWidget->InitializeData(PSArray[i]);
	}
}