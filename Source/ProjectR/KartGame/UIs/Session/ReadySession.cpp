#include "ReadySession.h"

#include "OnlineSessionSettings.h"
#include "SessionUtil.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/GameStateBase.h"
#include "KartGame/Games/Modes/Session/SessionGameState.h"
#include "Module/ReadySessionPlayerWidget.h"

void UReadySession::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	InitializeWidget();
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

void UReadySession::InitializeData()
{
	const FNamedOnlineSession* NamedOnlineSession = FSessionUtil::GetCurrentSession();
	if (!NamedOnlineSession)
	{  
		return; 
	}

	FString RoomTitleData;
	NamedOnlineSession->SessionSettings.Get(TEXT("RoomName"), RoomTitleData);
	
	RoomId->SetText(FText::FromString(NamedOnlineSession->GetSessionIdStr().Left(7)));
	RoomTitle->SetText(FText::FromString(FSessionUtil::DecodeData(RoomTitleData)));
}

void UReadySession::UpdatePlayers(const TArray<FString>& PlayerList)
{
	for (int i = 0; i < PlayerList.Num(); i++)
	{
		UReadySessionPlayerWidget* ChildWidget = Cast<UReadySessionPlayerWidget>(
			PlayerInfoGrid->GetChildAt(i));
		if (!ChildWidget)
		{
			return;
		}
		ChildWidget->InitializeData(PlayerList[i]);
	}
}