#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartLobbyUI.generated.h"

class ULicenseUI;
class UGameStartCinematicUI;
class UWidgetSwitcher;

UCLASS()
class PROJECTR_API UGameStartLobbyUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> UISwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameStartCinematicUI> StartCinematicUI;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULicenseUI> LicenseUI;

	UFUNCTION()
	void GoNextSubUI();
	
	UFUNCTION()
	void EndSubUI();
};
