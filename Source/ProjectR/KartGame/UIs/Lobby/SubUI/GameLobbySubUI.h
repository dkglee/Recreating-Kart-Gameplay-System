#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Blueprint/UserWidget.h"
#include "GameLobbySubUI.generated.h"

class UButton;

UCLASS()
class PROJECTR_API UGameLobbySubUI : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, GameMoveButton)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameMoveButton;
};
