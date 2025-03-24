#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickInputKey_Notified);

enum class ELobbyUI : uint8;

class UGameModeListUI;
class UInputAction;
class UInputMappingContext;
class UMainLobbyUI;

UCLASS()
class PROJECTR_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	FOnClickInputKey_Notified OnClickInputKey_C_Notified;
	FOnClickInputKey_Notified OnClickInputKey_F5_Notified;
	FOnClickInputKey_Notified OnClickInputKey_ESC_Notified;

	void PushWidgetStack(const ELobbyUI& LobbyUIKey);
	void PopWidgetStack();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	// TODO: 모듈화
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetStack;

#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> IMC_Lobby;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input|Key", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_C;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input|Key", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_F5;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_ExitWidget;
	
	void OnTrigger_C();
	void OnTrigger_F5();
	void OnTrigger_ExitWidget();
#pragma endregion
#pragma region WidgetInfo
	UPROPERTY()
	TObjectPtr<UMainLobbyUI> MainLobbyUI;
	
	UPROPERTY()
	TMap<ELobbyUI, TObjectPtr<UUserWidget>> WidgetValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainLobbyUI> MainLobbyUIClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameModeListUI> GameModeListUIClass;
#pragma endregion 
};
