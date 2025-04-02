#pragma once

#include "CoreMinimal.h"
#include "EnumUtil.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "SessionCreatePopupWidget.generated.h"

enum class EMatchType : uint8;
class UTextBlock;
class USlider;
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
	EMatchType SelectedMode = EMatchType::Item;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> RoomTitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> RoomPlayerCountSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoomPlayerCount;

	UFUNCTION()
	void OnChangePlayerCount(float Value);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> GamePublicCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateRoomButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ModeText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ModeSwitchButton;

#pragma region SessionCreateDelegate
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	
	UFUNCTION()
	void OnClickCreateRoomButton();

	UFUNCTION()
	void OnClickModeSwitchButton();

	void OnSessionCreated(FName SessionName, bool IsCreateSuccess);
#pragma endregion

#pragma region PrivateReference
	UPROPERTY()
	ALobbyPlayerController* PC;
#pragma endregion
};
