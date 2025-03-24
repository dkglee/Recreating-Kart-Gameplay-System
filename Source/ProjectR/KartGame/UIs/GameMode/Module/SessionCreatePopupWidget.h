#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "SessionCreatePopupWidget.generated.h"

class ALobbyPlayerController;
class UCheckBox;
class UButton;
class USpinBox;
class UEditableText;

UCLASS()
class PROJECTR_API USessionCreatePopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OpenSessionCreate();
	
	UFUNCTION()
	void RemoveSessionCreate();
	
protected:
	virtual void NativeOnInitialized() override;
	
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

#pragma region PrivateReference
	UPROPERTY()
	ALobbyPlayerController* PC;
#pragma endregion
};
