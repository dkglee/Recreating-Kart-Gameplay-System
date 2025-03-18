#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGameModeSelectList.generated.h"

class UScrollBox;

UCLASS()
class PROJECTR_API UItemGameModeSelectList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> RoomList;
};
