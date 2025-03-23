#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemModeListUI.generated.h"

class UScrollBox;

UCLASS()
class PROJECTR_API UItemModeListUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateToSearch();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SessionList;
};
