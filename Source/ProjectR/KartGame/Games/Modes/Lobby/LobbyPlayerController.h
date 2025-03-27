#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UWidgetControlComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickInputKey_Notified);

enum class ELobbyUI : uint8;

class UGameModeListUI;
class UInputMappingContext;
class UMainLobbyUI;

UCLASS()
class PROJECTR_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();
	
	GETTER(TObjectPtr<UWidgetControlComponent>, WidgetControlComponent)
	
protected:
	virtual void BeginPlay() override;
	
private:
	// TODO: 모듈화
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetStack;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> IMC_Lobby;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Control", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetControlComponent> WidgetControlComponent;
	
#pragma region WidgetInfo
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainLobbyUI> MainLobbyUIClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameModeListUI> GameModeListUIClass;
#pragma endregion 
};