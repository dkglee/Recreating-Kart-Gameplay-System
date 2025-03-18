#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainLobbyUI.generated.h"

class UGameLobbySubUI;
class UWidgetSwitcher;

UCLASS()
class PROJECTR_API UMainLobbyUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> LobbyUISwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameLobbySubUI> GameLobbySubUI;

	UFUNCTION()
	void CreateGameSessions();
};
