#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/_Common/WidgetStackInterface.h"
#include "GameModeListUI.generated.h"

class USessionCreatePopupWidget;
class UButton;
class UItemModeListUI;
class UWidgetSwitcher;

UCLASS()
class PROJECTR_API UGameModeListUI : public UUserWidget, public IWidgetStackInterface
{
	GENERATED_BODY()

public:
	virtual void SetDefaultWidgetInfo() override;
	virtual void ClearWidgetInfo() override;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> GameModeSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemModeListUI> ItemModeListUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SessionCreateButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USessionCreatePopupWidget> SessionCreatePopup;

	UFUNCTION()
	void OpenSessionCreate();
	
	UFUNCTION()
	void RemoveSessionCreate();
};
