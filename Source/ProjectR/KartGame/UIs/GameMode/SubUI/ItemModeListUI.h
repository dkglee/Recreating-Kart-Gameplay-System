#pragma once

#include "CoreMinimal.h"
#include "SessionUtil.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "ItemModeListUI.generated.h"

class USessionRoomWidget;
class UUniformGridPanel;
class FOnlineSessionSearch;
class UScrollBox;

UCLASS()
class PROJECTR_API UItemModeListUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSearchType();
	void UpdateToSearch();

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SessionList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SessionGridPanel;

#pragma region GridPanelOptions
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<USessionRoomWidget> SessionRoomWidgetClass;
	
	const uint8 Horizon = 2;
	const uint8 MaxCount = 30;
#pragma endregion
	FSessionSearchData SessionSearchData;

	UFUNCTION()
	void OnCompleteSearch(bool IsSuccess);
};