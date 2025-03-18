#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemGameChannelList.generated.h"

class UButton;

UCLASS()
class PROJECTR_API UItemGameChannelList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateSessionButton;

	UFUNCTION()
	void CreateNewSession();
};
