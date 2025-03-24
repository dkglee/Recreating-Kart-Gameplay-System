#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "SessionCreatePopupWidget.generated.h"

class UCheckBox;
class UButton;
class USpinBox;
class UEditableText;

UCLASS()
class PROJECTR_API USessionCreatePopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> RoomTitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> RoomPlayerCounter;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> GamePublicCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateRoomButton;

#pragma region SessionCreateDelegate
	UFUNCTION()
	void OnClickCreateRoomButton();

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	void OnSessionCreated(FName SessionName, bool IsCreateSuccess);
#pragma endregion 
};
